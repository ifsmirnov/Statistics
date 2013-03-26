#include <QApplication>

#include <iostream>

#include "window.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Window *window = new Window;
    window->show();

    return app.exec();
}
