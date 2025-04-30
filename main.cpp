#include <QApplication>
#include "ReversiGame.h"
#include "MyReversiContext.h"
#include "MyReversiInput.h"
#include "MyReversiView.h"

using namespace Reversi;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Initialize Context and View
    MyReversiContext context;
    MyReversiView view;
    view.Initialize();  // 確保這裡建立 ReversiWindow

    // Initialize Input with View's Window
    MyReversiInput input(view.getwindow(), &view);  // 注意這裡拿 view 的 window！

    // Create the game core
    ReversiGame game(&context, &input, &view);

    // Execute the game
    game.Execute();

    return app.exec();
}
