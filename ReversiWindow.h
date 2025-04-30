// ReversiWindow.h
#ifndef REVERSI_WINDOW_H
#define REVERSI_WINDOW_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <vector>
#include "IReversiContext.h"

namespace Reversi {

constexpr int BOARD_SIZE = 8;

class GridButton : public QPushButton {
    Q_OBJECT

public:
    GridButton(int row, int col, QWidget* parent = nullptr);

    int row() const { return m_row; }
    int col() const { return m_col; }

signals:
    void Hovered(int row, int col);
    void Clicked(int row, int col);

protected:
    void enterEvent(QEnterEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    int m_row;
    int m_col;
};

class ReversiWindow : public QWidget {
    Q_OBJECT

public:
    ReversiWindow(QWidget* parent = nullptr);

    void updateDisk(int row, int col, char color);
    void resetBoard();

    void setTurnText(const QString& text);
    void updateScoreLabel(IReversiContext* context);
    void updateTimer();
    void startTurnTimer();
    void stopTurnTimer();
    void showWinner(GAME_STATE state);
    void hideWinner();    
    const std::vector<std::vector<GridButton*>>& buttons() const { return m_buttons; }
    void setTimeoutCallback(std::function<void()> callback);
signals:
    void RestartRequested();

private:
    std::vector<std::vector<GridButton*>> m_buttons;

    QVBoxLayout* m_mainLayout;
    QWidget* m_boardWidget;
    QGridLayout* m_boardLayout;
    QLabel* m_turnLabel;
    QLabel* m_scoreLabel;
    // Timer
    QTimer* m_turnTimer;
    QLabel* m_timerLabel;
    int m_remainingSeconds;
    // Winner
    QLabel* winnerLabel;
    QPushButton* restartButton;

    std::function<void()> timeoutCallback;
};

} // namespace Reversi

#endif // REVERSI_WINDOW_H
