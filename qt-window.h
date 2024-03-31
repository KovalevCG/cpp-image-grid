#pragma once

#include <QMainWindow>
#include <QPushButton>
#include "global-resources.h"

class QtWindow : public QMainWindow {
    Q_OBJECT

public:
    QtWindow(QWidget* parent = nullptr);

private:
    void onEditButtonClicked();
    void setupUi();
    void quitApp();
};