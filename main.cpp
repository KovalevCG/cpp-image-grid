// MAIN PROJECT

#include "qtwindow.h"
#include <QApplication>

// Console Output
// Comment it to remove console
#include <fstream>
#include <Windows.h>
#include <iostream>

int main(int argc, char* argv[]) {

    // Console Output
    // Comment it to remove console
    AllocConsole();
    FILE* stream;
    stream = freopen("CONOUT$", "w", stdout);
    stream = freopen("CONOUT$", "w", stderr);
    std::cout << "Console output enabled.\n";

    QApplication app(argc, argv);
    QtWindow window;
    window.show();
    return app.exec();
}