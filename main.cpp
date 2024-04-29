// MAIN FILE OF PROJECT

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

#include "qt-window.h"
#include "global-resources.h"

#include <QApplication>

// Console Output
#include <fstream>
#include <Windows.h>
#include <iostream>

// Check Windows Version
typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
void CheckWindowsVersion() {
    HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
    if (hMod) {
        RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)::GetProcAddress(hMod, "RtlGetVersion");
        if (fxPtr != nullptr) {
            RTL_OSVERSIONINFOW rovi = {};
            rovi.dwOSVersionInfoSize = sizeof(rovi);
            if (NT_SUCCESS(fxPtr(&rovi))) {
                GlobalResources::WIN_VERSION = rovi.dwMajorVersion;
            }
        }
    }
}



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
    // Check Windows Version
    CheckWindowsVersion();

    QtWindow window;
    app.setProperty("mainWindow", QVariant::fromValue<QWidget*>(&window));
    window.show();
    return app.exec();
} 

