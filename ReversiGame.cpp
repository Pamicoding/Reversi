// ReversiGame.cpp

#include "ReversiGame.h"
#include "MyReversiContext.h"
#include <vector>
#include <iostream>

namespace Reversi {

// --- Constructor ---
ReversiGame::ReversiGame(IReversiContext* ctx, IReversiInput* inp, IReversiView* vw)
    : context(ctx), input(inp), view(vw) {}

// --- Execute ---
void ReversiGame::Execute() {
    // view->Initialize();
    view->SetRestartCallback([this]() {
        RestartGame();
    });

    view->SetTimeoutCallback([this]() {
        HandleTimeout();
    });    

    StartGame();
}

// --- StartGame ---
void ReversiGame::StartGame() {
    input->SetInputLock(true);
    view->ShowGameStarting(context, [=]() {
        input->SetInputLock(false);
        WaitForPlayerInput();
    });
}

// --- WaitForPlayerInput ---
void ReversiGame::WaitForPlayerInput() {
    input->SetInputLock(false);

    // 標出可以下的位置
    BoardPositionVector placeablePositions;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (context->GetSlotStatus(i, j) == SLOT_STATUS::EMPTY) {
                BoardPositionVector capturable = GetFlippableDisks({i, j});
                if (!capturable.empty()) {
                    placeablePositions.push_back({i, j});
                }
            }
        }
    }
    view->ShowPlaceablePreview(context->GetCurrentPlayerColor(), placeablePositions);

    // 等待玩家輸入
    input->WaitPreviewInput([=](BoardPosition position) {
        view->ClearCapturablePreview();
        BoardPositionVector capturable = GetFlippableDisks(position);

        std::cout << "[Debug] Hover at (" << position.first << "," << position.second << "), capturable size = " << capturable.size() << std::endl;

        view->ShowCapturablePreview(capturable);
    });

    input->WaitPlacingInput([=](BoardPosition position) {
        if (ProcessDiskPlacement(position)) {
            input->SetInputLock(true);
            view->ClearPlaceablePreview();
            view->ClearCapturablePreview();
        }
    });
}

// --- GetFlippableDisks ---
BoardPositionVector ReversiGame::GetFlippableDisks(BoardPosition placingPosition) {
    BoardPositionVector flippable;

    int row = placingPosition.first;
    int col = placingPosition.second;

    SLOT_STATUS myDisk;
    if (context->GetCurrentState() == GAME_STATE::BLACK_TURN) {
        myDisk = SLOT_STATUS::BLACK;
    } else if (context->GetCurrentState() == GAME_STATE::WHITE_TURN) {
        myDisk = SLOT_STATUS::WHITE;
    } else {
        return flippable;
    }

    SLOT_STATUS enemyDisk = (myDisk == SLOT_STATUS::BLACK) ? SLOT_STATUS::WHITE : SLOT_STATUS::BLACK;

    const int dx[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
    const int dy[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

    for (int dir = 0; dir < 8; ++dir) {
        int x = row + dx[dir];
        int y = col + dy[dir];

        BoardPositionVector temp;

        while (x >= 0 && x < 8 && y >= 0 && y < 8) {
            SLOT_STATUS status = context->GetSlotStatus(x, y);
            if (status == enemyDisk) {
                temp.push_back({x, y});
            } else if (status == myDisk) {
                if (!temp.empty()) {
                    flippable.insert(flippable.end(), temp.begin(), temp.end());
                }
                break;
            } else {
                break;
            }

            x += dx[dir];
            y += dy[dir];
        }
    }

    return flippable;
}

// --- ProcessDiskPlacement ---
bool ReversiGame::ProcessDiskPlacement(BoardPosition placingPosition) {
    SLOT_STATUS currentSlot = context->GetSlotStatus(placingPosition.first, placingPosition.second);
    if (currentSlot != SLOT_STATUS::EMPTY) {
        return false;
    }

    BoardPositionVector capturableDisks = GetFlippableDisks(placingPosition);
    if (capturableDisks.empty()) {
        return false;
    }

    SLOT_STATUS myDisk = (context->GetCurrentState() == GAME_STATE::BLACK_TURN) ? SLOT_STATUS::BLACK : SLOT_STATUS::WHITE;

    context->SetSlotStatus(placingPosition.first, placingPosition.second, myDisk);

    view->ShowDiskPlacing((myDisk == SLOT_STATUS::BLACK) ? PLAYER_COLOR::BLACK : PLAYER_COLOR::WHITE, placingPosition, [=]() {
        for (auto& pos : capturableDisks) {
            context->SetSlotStatus(pos.first, pos.second, myDisk);
        }

        view->ShowDiskFlipping(capturableDisks, [=]() {
            ProcessStateChange();
        });
    });

    return true;
}

// --- ProcessStateChange ---
void ReversiGame::ProcessStateChange() {
    auto hasValidMove = [&](SLOT_STATUS playerDisk) -> bool {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (context->GetSlotStatus(i, j) == SLOT_STATUS::EMPTY) {
                    BoardPositionVector test = GetFlippableDisks({i, j});
                    if (!test.empty()) {
                        return true;
                    }
                }
            }
        }
        return false;
    };

    GAME_STATE prevState = context->GetCurrentState();
    GAME_STATE nextState;

    if (prevState == GAME_STATE::BLACK_TURN) {
        nextState = GAME_STATE::WHITE_TURN;
    } else if (prevState == GAME_STATE::WHITE_TURN) {
        nextState = GAME_STATE::BLACK_TURN;
    } else {
        return; // 遊戲已結束
    }

    context->SetCurrentState(nextState);

    SLOT_STATUS nextDisk = (nextState == GAME_STATE::BLACK_TURN) ? SLOT_STATUS::BLACK : SLOT_STATUS::WHITE;

    if (!hasValidMove(nextDisk)) {
        // 對方不能下，換回自己
        nextState = prevState;
        context->SetCurrentState(nextState);

        SLOT_STATUS selfDisk = (nextState == GAME_STATE::BLACK_TURN) ? SLOT_STATUS::BLACK : SLOT_STATUS::WHITE;

        if (!hasValidMove(selfDisk)) {
            // 雙方都不能下，遊戲結束，計分
            int blackCount = 0, whiteCount = 0;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (context->GetSlotStatus(i, j) == SLOT_STATUS::BLACK) {
                        blackCount++;
                    } else if (context->GetSlotStatus(i, j) == SLOT_STATUS::WHITE) {
                        whiteCount++;
                    }
                }
            }

            if (blackCount > whiteCount) {
                context->SetCurrentState(GAME_STATE::BLACK_WON);
            } else if (whiteCount > blackCount) {
                context->SetCurrentState(GAME_STATE::WHITE_WON);
            } else {
                context->SetCurrentState(GAME_STATE::DRAW);
            }

            view->ShowStateChanging(context->GetCurrentState(), prevState, [=]() {
                view->ShowWinner(context->GetCurrentState());
            });
            return;
        }
    }

    view->ShowStateChanging(nextState, prevState, [=]() {
        WaitForPlayerInput();
    });
}

// --- RestartGame ---
void ReversiGame::RestartGame() {
    context->ResetBoard();        // 1. 盤面 reset
    context->SetCurrentState(GAME_STATE::BLACK_TURN); // 2. 黑方先手
    view->ResetWindow(context);   // 3. UI全部 reset
    Execute(); 
}

void ReversiGame::HandleTimeout() {
    input->SetInputLock(true);

    const auto& legalMoves = view->getPlaceablePreviewPositions();

    if (!legalMoves.empty()) {
        BoardPosition choice = legalMoves[rand() % legalMoves.size()];
        ProcessDiskPlacement(choice);
        view->ClearPlaceablePreview();
    } else {
        ProcessStateChange();
    }
}

} // namespace Reversi
