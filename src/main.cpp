#include <QApplication>
#include "window.h"


int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    Window *window = new Window();

    window->setWindowTitle("Steam Game Mover");
    window->setFixedSize(320, 420);
    window->show();

    return app.exec();
}
