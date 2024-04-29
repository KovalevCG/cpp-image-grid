#pragma once

#include "opencv.h"

#include <QMainWindow>
#include <QPushButton>

class QtWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit QtWindow(QWidget* parent = nullptr); //explicit added
    void saveFileDialog();

private:
    void onEditButtonClicked();
    void setupUi();
    void quitApp();
    void showAbout();
    void runOcvTest();
};