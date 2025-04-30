# ReversiGame.pro

# 告訴 qmake：這是一個 Qt GUI 專案
QT += core gui widgets

# 編譯器設定
CONFIG += c++17
CONFIG += warn_on

# 指定輸出執行檔名字
TARGET = reversi_game

# 生成執行檔 (app)
TEMPLATE = app

# 你的原始碼（cpp檔）
SOURCES += \
    main.cpp \
    ReversiGame.cpp \
    ReversiWindow.cpp \
    MyReversiContext.cpp \
    MyReversiInput.cpp \
    MyReversiView.cpp

# 你的標頭檔案（h檔）
HEADERS += \
    ReversiDefine.h \
    ReversiWindow.h \
    ReversiGame.h \
    IReversiView.h \
    IReversiInput.h \
    IReversiContext.h \
    MyReversiContext.h \
    MyReversiInput.h \
    MyReversiView.h

# 這樣 qmake 自動處理 moc (處理 Q_OBJECT)
