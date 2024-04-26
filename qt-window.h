#pragma once

#include <QMainWindow>
#include <QPushButton>

class QtWindow : public QMainWindow {
    Q_OBJECT

public:
    QtWindow(QWidget* parent = nullptr);

private:
    void onEditButtonClicked();
    void setupUi();
    void quitApp();
    void showAbout();
    void runOcvTest();
};