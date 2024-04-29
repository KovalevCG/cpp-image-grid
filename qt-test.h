#pragma once

#include <QMainWindow>
#include <QPushButton>
#include "ocv-test.h"

class QtTest : public QMainWindow {
    Q_OBJECT

public:
    explicit QtTest(QWidget* parent = nullptr);
    ~QtTest();
    void showAbout();

private:
    void setupUi();
    QPushButton* startButton;
    QPushButton* aboutButton;
    OcvTest* ocvTestWindow;
};


