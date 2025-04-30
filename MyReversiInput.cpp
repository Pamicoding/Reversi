// MyReversiInput.cpp
#include "MyReversiInput.h"
#include <iostream>
#include <QTimer>
namespace Reversi {

MyReversiInput::MyReversiInput(ReversiWindow* window, MyReversiView* view)
    : m_window(window), m_view(view), inputLocked(true)
{
    std::cout << "[Debug] MyReversiInput constructor START" << std::endl;
    const auto& buttons = m_window->buttons();
    std::cout << "[Debug] buttons size: " << buttons.size() << std::endl;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        std::cout << "[Debug] row " << row << " has " << buttons[row].size() << " columns." << std::endl;
        for (int col = 0; col < BOARD_SIZE; ++col) {
            auto button = buttons[row][col];
            if (button) {
                bool ok = connect(button, SIGNAL(Hovered(int,int)), this, SLOT(OnHover(int,int)));
                if (!ok) {
                    std::cout << "[Debug] connect Hovered FAILED at (" << row << "," << col << ")" << std::endl;
                }
                ok = connect(button, SIGNAL(Clicked(int,int)), this, SLOT(OnClick(int,int)));
                if (!ok) {
                    std::cout << "[Debug] connect Clicked FAILED at (" << row << "," << col << ")" << std::endl;
                }
            } else {
                std::cout << "[Debug] No button at (" << row << "," << col << ")" << std::endl;
            }
        }
    }
    std::cout << "[Debug] MyReversiInput constructor END" << std::endl;
    // QTimer::singleShot(1000, [=]() {
    //     std::cout << "[Debug] QTimer forced emit Hovered(0,0)" << std::endl;
    //     emit buttons[0][0]->Hovered(0, 0);  // 模擬hover一下 (0,0)
    // });
}

MyReversiInput::~MyReversiInput() {}

void MyReversiInput::SetInputLock(bool lockState) {
    inputLocked = lockState;
}

void MyReversiInput::WaitPreviewInput(PositionCallback callback) {
    previewCallback = callback;
}

void MyReversiInput::WaitPlacingInput(PositionCallback callback) {
    placingCallback = callback;
}

void MyReversiInput::OnHover(int row, int col) {
    if (!inputLocked) {
        bool isPlaceable = false;
        for (const auto& pos : m_view->getPlaceablePreviewPositions()) {
            if (pos.first == row && pos.second == col) {
                isPlaceable = true;
                break;
            }
        }

        if (isPlaceable) {
            if (previewCallback) previewCallback({row, col});
        } else {
            if (clearCallback) clearCallback({row, col});
        }
    }
}

void MyReversiInput::OnClick(int row, int col) {
    if (!inputLocked && placingCallback) {
        placingCallback({row, col});
    }
}

} // namespace Reversi
