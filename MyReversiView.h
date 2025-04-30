// MyReversiView.h
#ifndef MY_REVERSI_VIEW_H
#define MY_REVERSI_VIEW_H

#include "IReversiView.h"
#include "ReversiWindow.h"
#include <memory>

namespace Reversi {

class MyReversiView : public IReversiView {
public:
    MyReversiView();
    ~MyReversiView();

    void Initialize() override;
    void ShowGameStarting(IReversiContext* ctx, Callback endCallback) override;
    void ShowDiskPlacing(PLAYER_COLOR color, BoardPosition position, Callback endCallback) override;
    void ShowDiskFlipping(const BoardPositionVector& positions, Callback endCallback) override;
    void ShowStateChanging(GAME_STATE state, GAME_STATE prevState, Callback endCallback) override;
    void ShowPlaceablePreview(PLAYER_COLOR targetColor, const BoardPositionVector& positions) override;
    void ClearPlaceablePreview() override;
    void ShowCapturablePreview(const BoardPositionVector& positions) override;
    void ClearCapturablePreview() override;
    void SetRestartCallback(Callback cb) override;
    void ShowWinner(GAME_STATE state) override;
    void HideWinner();
    void ResetWindow(IReversiContext* context) override;
    void SetTimeoutCallback(std::function<void()> cb) override;
    QString stateToString(GAME_STATE state);

    ReversiWindow* getwindow() { return window.get(); }
    const std::vector<BoardPosition>& getPlaceablePreviewPositions() const override {
    return placeablePreviewPositions;
}
private:
    IReversiContext* context = nullptr;
    std::vector<std::pair<BoardPosition, char>> capturablePreviewStates;
    std::vector<BoardPosition> placeablePreviewPositions;
    std::unique_ptr<ReversiWindow> window;
    Callback restartCallback;
    std::function<void()> timeoutCallback; 
};

} // namespace Reversi

#endif // MY_REVERSI_VIEW_H

