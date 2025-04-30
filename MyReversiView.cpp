// MyReversiView.cpp
#include "MyReversiView.h"
#include <QTimer>
#include <iostream>

namespace Reversi {

MyReversiView::MyReversiView() {}

MyReversiView::~MyReversiView() {}

void MyReversiView::Initialize() {
    window = std::make_unique<ReversiWindow>();
    window->show();
    
    QObject::connect(window.get(), &ReversiWindow::RestartRequested, [this]() {
        if (restartCallback) {
            restartCallback();
        }
    });

    window->setTimeoutCallback([this]() {
    if (timeoutCallback) timeoutCallback(); 
    });        
}

void MyReversiView::ShowGameStarting(IReversiContext* ctx, Callback endCallback) {
    context = ctx;

    window->resetBoard();
    window->updateDisk(3, 3, 'W');
    window->updateDisk(3, 4, 'B');
    window->updateDisk(4, 3, 'B');
    window->updateDisk(4, 4, 'W');

    QTimer::singleShot(500, [endCallback]() {
        if (endCallback) endCallback();
    });
}

void MyReversiView::ShowDiskPlacing(PLAYER_COLOR color, BoardPosition position, Callback endCallback) {
    char diskColor = (color == PLAYER_COLOR::BLACK) ? 'B' : 'W';

    // 把position 從 placeablePreviewPositions 移除
    auto it = std::remove_if(placeablePreviewPositions.begin(), placeablePreviewPositions.end(),
        [&](const BoardPosition& pos) {
            return pos == position;
        });
    placeablePreviewPositions.erase(it, placeablePreviewPositions.end());

    // 更新棋盤
    window->updateDisk(position.first, position.second, diskColor);

    if (window && context) {
        window->updateScoreLabel(context);
    }

    // 執行 callback
    QTimer::singleShot(300, [endCallback, this]() {
        if (window) window->stopTurnTimer();
        if (endCallback) endCallback();
    });
}

void MyReversiView::ShowDiskFlipping(const BoardPositionVector& positions, Callback endCallback) {
    if (!context) {
        if (endCallback) endCallback();
        return;
    }

    char diskColor = 'B';
    GAME_STATE currentState = context->GetCurrentState();
    if (currentState == GAME_STATE::WHITE_TURN) {
        diskColor = 'W';
    }

    for (const auto& pos : positions) {
        window->updateDisk(pos.first, pos.second, diskColor);
    }

    if (window && context) {
        window->updateScoreLabel(context);
    }

    QTimer::singleShot(300, [endCallback]() {
        if (endCallback) endCallback();
    });
}

void MyReversiView::ShowStateChanging(GAME_STATE state, GAME_STATE prevState, Callback endCallback) {
    if (window) {
        if (state == GAME_STATE::BLACK_TURN) {
            window->setTurnText("Black's Turn");
        } else if (state == GAME_STATE::WHITE_TURN) {
            window->setTurnText("White's Turn");
        }
        window->startTurnTimer();
    }

    QTimer::singleShot(300, [endCallback]() {
        if (endCallback) endCallback();
    });
}

void MyReversiView::ShowPlaceablePreview(PLAYER_COLOR targetColor, const BoardPositionVector& positions) {
    placeablePreviewPositions = positions;
    for (const auto& pos : positions) {
        window->updateDisk(pos.first, pos.second, 'G');
    }
}

void MyReversiView::ClearPlaceablePreview() {
    for (const auto& pos : placeablePreviewPositions) {
        window->updateDisk(pos.first, pos.second, 'E'); // 還原
    }
    placeablePreviewPositions.clear(); // 清空記錄
}

void MyReversiView::ShowCapturablePreview(const BoardPositionVector& positions) {
    capturablePreviewStates.clear();
    for (const auto& pos : positions) {
        SLOT_STATUS status = context->GetSlotStatus(pos.first, pos.second);
        char originalColor;
        if (status == SLOT_STATUS::BLACK) {
            originalColor = 'B';
        } else if (status == SLOT_STATUS::WHITE) {
            originalColor = 'W';
        } else {
            originalColor = 'E';
        }
        capturablePreviewStates.push_back({pos, originalColor});
        window->updateDisk(pos.first, pos.second, 'Y');
    }
}

void MyReversiView::ClearCapturablePreview() {
    for (const auto& entry : capturablePreviewStates) {
        BoardPosition pos = entry.first;
        char originalColor = entry.second;

        window->updateDisk(pos.first, pos.second, originalColor);
    }
    capturablePreviewStates.clear();
}

void MyReversiView::SetRestartCallback(Callback cb) {
    restartCallback = cb;
}

void MyReversiView::ShowWinner(GAME_STATE state) {
    if (window) {
        window->showWinner(state);
    }
}

void MyReversiView::HideWinner() {
    if (window) {
        window->hideWinner();
    }
}

QString MyReversiView::stateToString(GAME_STATE state) {
    switch (state) {
    case GAME_STATE::BLACK_TURN: return "Black's Turn";
    case GAME_STATE::WHITE_TURN: return "White's Turn";
    case GAME_STATE::BLACK_WON: return "Black Wins!";
    case GAME_STATE::WHITE_WON: return "White Wins!";
    case GAME_STATE::DRAW: return "Draw!";
    default: return "Unknown";
    }
}

void MyReversiView::ResetWindow(IReversiContext* context) {
    if (!window) return;

    window->hideWinner();    // 隱藏獲勝標籤
    window->resetBoard();    // 棋盤顏色回初始 (全部綠色)
    window->updateScoreLabel(context); // 更新成2:2
    window->setTurnText(stateToString(context->GetCurrentState())); // 顯示 "Black's Turn"
    window->updateTimer();    // 時間重置30秒
}

void MyReversiView::SetTimeoutCallback(std::function<void()> cb) {
    timeoutCallback = std::move(cb);
}
} // namespace Reversi
