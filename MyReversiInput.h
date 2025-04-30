// MyReversiInput.h
#ifndef MY_REVERSI_INPUT_H
#define MY_REVERSI_INPUT_H

#include "MyReversiView.h"
#include "IReversiInput.h"
#include "ReversiWindow.h"
#include <QObject>
#include <QString>
#include <functional>

namespace Reversi {

class MyReversiInput : public QObject, public IReversiInput {
    Q_OBJECT

public:
    MyReversiInput(ReversiWindow* window, MyReversiView* view);
    ~MyReversiInput();

    void WaitPreviewInput(PositionCallback callback) override;
    void WaitPlacingInput(PositionCallback callback) override;
    void SetInputLock(bool lockState) override;

private slots:
    void OnHover(int row, int col);
    void OnClick(int row, int col);

private:
    MyReversiView* m_view;
    ReversiWindow* m_window;
    bool inputLocked;
    PositionCallback previewCallback;
    PositionCallback placingCallback;
    PositionCallback clearCallback;
};

} // namespace Reversi

#endif // MY_REVERSI_INPUT_H