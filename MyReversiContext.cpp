// MyReversiContext.cpp

#include "MyReversiContext.h"

namespace Reversi {

MyReversiContext::MyReversiContext() {
    ResetBoard();
    currentState = GAME_STATE::BLACK_TURN;
}

SLOT_STATUS MyReversiContext::GetSlotStatus(int row, int col) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return board[row][col];
    }
    return SLOT_STATUS::EMPTY; // 異常輸入回傳 EMPTY
}

SLOT_STATUS MyReversiContext::SetSlotStatus(int row, int col, SLOT_STATUS status) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        board[row][col] = status;
    }
    return status;
}

GAME_STATE MyReversiContext::GetCurrentState() {
    return currentState;
}

GAME_STATE MyReversiContext::SetCurrentState(GAME_STATE state) {
    currentState = state;
    return currentState;
}

PLAYER_COLOR MyReversiContext::GetCurrentPlayerColor() {
    if (currentState == GAME_STATE::BLACK_TURN) {
        return PLAYER_COLOR::BLACK;
    } else if (currentState == GAME_STATE::WHITE_TURN) {
        return PLAYER_COLOR::WHITE;
    } else {
        
        return PLAYER_COLOR::BLACK;
    }
}

void MyReversiContext::ResetBoard() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = SLOT_STATUS::EMPTY;
        }
    }
    board[3][3] = SLOT_STATUS::WHITE;
    board[4][4] = SLOT_STATUS::WHITE;
    board[3][4] = SLOT_STATUS::BLACK;
    board[4][3] = SLOT_STATUS::BLACK;
}

} // namespace Reversi
