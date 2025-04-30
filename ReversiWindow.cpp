// ReversiWindow.cpp
#include "ReversiWindow.h"
#include <QEnterEvent>
#include <QMouseEvent>
#include <iostream>

namespace Reversi {

GridButton::GridButton(int row, int col, QWidget* parent)
    : QPushButton(parent), m_row(row), m_col(col) {
    setFixedSize(60, 60);
    setStyleSheet("background-color: green; border: 1px solid black;");
    setMouseTracking(true); 
}

void GridButton::enterEvent(QEnterEvent* event) {
    std::cout << "[Debug] GridButton enterEvent at (" << m_row << "," << m_col << ")" << std::endl;
    emit Hovered(m_row, m_col);
    std::cout << "[Debug] GridButton emit Hovered at (" << m_row << ", " << m_col << ")" << std::endl;
    QPushButton::enterEvent(event);
}

void GridButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        std::cout << "[Debug] GridButton mousePressEvent at (" << m_row << "," << m_col << ")" << std::endl;
        emit Clicked(m_row, m_col);
    }
    QPushButton::mousePressEvent(event);
}

ReversiWindow::ReversiWindow(QWidget* parent)
    : QWidget(parent) {
    // --- Main Layout ---
    m_mainLayout = new QVBoxLayout(this);

    // --- Turn Label (誰的回合) ---
    m_turnLabel = new QLabel("Black's Turn", this);
    m_turnLabel->setAlignment(Qt::AlignCenter);
    m_mainLayout->addWidget(m_turnLabel);

    // --- Timer Label (倒數計時) ---
    m_timerLabel = new QLabel("Time: 10s", this);
    m_timerLabel->setAlignment(Qt::AlignCenter);
    m_timerLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    m_mainLayout->addWidget(m_timerLabel);

    // --- Board Widget ---
    m_boardWidget = new QWidget(this);
    m_boardLayout = new QGridLayout(m_boardWidget);
    m_boardLayout->setSpacing(0);
    m_boardLayout->setContentsMargins(0, 0, 0, 0);

    for (int row = 0; row < BOARD_SIZE; ++row) {
        std::vector<GridButton*> rowButtons;
        for (int col = 0; col < BOARD_SIZE; ++col) {
            GridButton* button = new GridButton(row, col, m_boardWidget);
            rowButtons.push_back(button);
            m_boardLayout->addWidget(button, row, col);
        }
        m_buttons.push_back(rowButtons);
    }
    m_mainLayout->addWidget(m_boardWidget);

    // --- Score Label (顯示雙方分數) ---
    m_scoreLabel = new QLabel("Black: 2, White: 2", this);
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    m_scoreLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    m_mainLayout->addWidget(m_scoreLabel);

    // --- Winner Label (贏家顯示，預設隱藏) ---
    winnerLabel = new QLabel(this);
    winnerLabel->setAlignment(Qt::AlignCenter);
    winnerLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: red;");
    winnerLabel->hide();
    m_mainLayout->addWidget(winnerLabel);
    m_mainLayout->setAlignment(winnerLabel, Qt::AlignCenter);

    // --- Restart Button (重新開始按鈕，預設隱藏) ---
    restartButton = new QPushButton("Restart Game", this);
    restartButton->setFixedSize(150, 50);
    restartButton->hide();
    m_mainLayout->addWidget(restartButton);
    m_mainLayout->setAlignment(restartButton, Qt::AlignCenter);
    connect(restartButton, &QPushButton::clicked, this, &ReversiWindow::RestartRequested);

    // --- Timer 物件設定 ---
    m_turnTimer = new QTimer(this);
    connect(m_turnTimer, &QTimer::timeout, this, &ReversiWindow::updateTimer);
    m_remainingSeconds = 10;

    // --- Finish Setting ---
    setLayout(m_mainLayout);
    setWindowTitle("Reversi Game");
    resize(500, 500); 
}

void ReversiWindow::updateDisk(int row, int col, char color) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) return;
    if (!m_buttons[row][col]) return;

    if (color == 'B') {
        m_buttons[row][col]->setText("●");
        m_buttons[row][col]->setStyleSheet("background-color: darkgreen; color: black; font-size: 24px; border: 1px solid black;");
    } else if (color == 'W') {
        m_buttons[row][col]->setText("●");
        m_buttons[row][col]->setStyleSheet("background-color: darkgreen; color: white; font-size: 24px; border: 1px solid black;");
    } else if (color == 'G') {
        m_buttons[row][col]->setText(""); 
        m_buttons[row][col]->setStyleSheet("background-color: lightgreen; border: 2px dashed black;");
    } else if (color == 'Y') {
        m_buttons[row][col]->setText(""); 
        m_buttons[row][col]->setStyleSheet("background-color: yellow; border: 2px dashed black;");
    } 
    else {
        m_buttons[row][col]->setText("");
        m_buttons[row][col]->setStyleSheet("background-color: green; border: 1px solid black;");
    }
}

void ReversiWindow::resetBoard() {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (m_buttons[row][col]) {
                m_buttons[row][col]->setStyleSheet("background-color: green; border: 1px solid black;");
                m_buttons[row][col]->setText("");
                m_buttons[row][col]->setIcon(QIcon());
            }
        }
    }
}

void ReversiWindow::setTurnText(const QString& text) {
    if (m_turnLabel) {
        m_turnLabel->setText(text);
    }
}

void ReversiWindow::updateScoreLabel(IReversiContext* context) {
    int black = 0, white = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            SLOT_STATUS status = context->GetSlotStatus(i, j);
            if (status == SLOT_STATUS::BLACK) black++;
            if (status == SLOT_STATUS::WHITE) white++;
        }
    }
    m_scoreLabel->setText(QString("Black: %1, White: %2").arg(black).arg(white));
}

void ReversiWindow::updateTimer() {
    m_remainingSeconds--;
    if (m_remainingSeconds <= 0) {
        m_turnTimer->stop();
        m_timerLabel->setText("Time's up!");

        if (timeoutCallback) timeoutCallback();

    } else {
        m_timerLabel->setText(QString("Time: %1s").arg(m_remainingSeconds));
    }
}

void ReversiWindow::startTurnTimer() {
    m_remainingSeconds = 10;
    m_timerLabel->setText("Time: 10s");
    m_turnTimer->start(1000); // 每1000ms呼叫一次 timeout
}

void ReversiWindow::stopTurnTimer() {
    m_turnTimer->stop();
}

void ReversiWindow::showWinner(GAME_STATE state) {
    if (state == GAME_STATE::BLACK_WON) {
        winnerLabel->setText("Black Wins!");
    } else if (state == GAME_STATE::WHITE_WON) {
        winnerLabel->setText("White Wins!");
    } else {
        winnerLabel->setText("It's a Draw!");
    }
    winnerLabel->show();
    restartButton->show();
}

void ReversiWindow::hideWinner() {
    winnerLabel->hide();
    restartButton->hide();
}

void ReversiWindow::setTimeoutCallback(std::function<void()> callback) {
    timeoutCallback = std::move(callback);
}

} // namespace Reversi
