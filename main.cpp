// MAIN PROJECT

#include "qt-window.h"
#include "global-resources.h"
#include "image-label.h"

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
    // ImageLabel Init
    // ImageLabel::initStaticPixmap();

    QtWindow window;
    window.show();
    return app.exec();
}