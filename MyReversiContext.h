// MyReversiContext.h
#ifndef MYREVERSICONTEXT_H
#define MYREVERSICONTEXT_H

#include "IReversiContext.h"
#include "ReversiDefine.h"

namespace Reversi {

class MyReversiContext : public IReversiContext {
private:
    SLOT_STATUS board[8][8];
    GAME_STATE currentState;

public:
    MyReversiContext();

    SLOT_STATUS GetSlotStatus(int row, int col) override;
    SLOT_STATUS SetSlotStatus(int row, int col, SLOT_STATUS status) override;
    GAME_STATE GetCurrentState() override;
    GAME_STATE SetCurrentState(GAME_STATE state) override;
    PLAYER_COLOR GetCurrentPlayerColor() override;

    void ResetBoard() override;
};

} // namespace Reversi

#endif // MYREVERSICONTEXT_H
