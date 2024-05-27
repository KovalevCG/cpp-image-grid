// MAIN FILE OF PROJECT

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

#include "qt-window.h"
#include "global-resources.h"

#include <QApplication>

//// Console Output
//#include <fstream>
#include <Windows.h>
// #include <iostream>

#include <iostream>
#include <chrono>
#include <ctime>

// Checking Windows Version
typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
void CheckWindowsVersion() {
    HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
    if (hMod) {
        RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)::GetProcAddress(hMod, "RtlGetVersion");
        if (fxPtr != nullptr) {
            RTL_OSVERSIONINFOW rovi = {};
            rovi.dwOSVersionInfoSize = sizeof(rovi);
            if (NT_SUCCESS(fxPtr(&rovi))) {
                if (rovi.dwBuildNumber >= 22000) {
                    GlobalResources::WIN_VERSION = 11;
                }
            }
            else {
            }
        }
        else {
        }
    }
    else {
    }
}

// Check Date
bool isDateLessThen() {

    // Get current system time as a time_point
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm* parts = std::localtime(&now_c);
    int year = 1900 + parts->tm_year;

    // Check the year
    if (year <= 2024) {
        return true;
    }
    else {
        return false;
    }
}


int main(int argc, char* argv[]) {

    if (!isDateLessThen()) {
        return 0;
    }

    // Qt Application
    QApplication app(argc, argv);

    //// Console Output
    //AllocConsole();
    //FILE* stream;
    //stream = freopen("CONOUT$", "w", stdout);
    //stream = freopen("CONOUT$", "w", stderr);
    //std::cout << "Console output enabled.\n";

    // GlobalResources Init
    GlobalResources::initGlobalResources();
    // Check Windows Version
    CheckWindowsVersion();

    QtWindow window;
    app.setProperty("mainWindow", QVariant::fromValue<QWidget*>(&window));
    window.show();
    return app.exec();
} 

