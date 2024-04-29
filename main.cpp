// MAIN PROJECT

#include "qt-window.h"
#include "global-resources.h"

#include <QApplication>

// Console Output
#include <fstream>
#include <Windows.h>
#include <iostream>

int main(int argc, char* argv[]) {

    // Qt Application
    QApplication app(argc, argv);

    // Console Output
    AllocConsole();
    FILE* stream;
    stream = freopen("CONOUT$", "w", stdout);
    stream = freopen("CONOUT$", "w", stderr);
     std::cout << "Console output enabled.\n";

    // GlobalResources Init
    GlobalResources::initGlobalResources();

    QtWindow window;
    app.setProperty("mainWindow", QVariant::fromValue<QWidget*>(&window));
    window.show();
    return app.exec();
} 