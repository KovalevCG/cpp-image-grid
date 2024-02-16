#pragma once

#include <QMainWindow>
#include <QPushButton>
#include "globalresources.h"

class QtWindow : public QMainWindow {
    Q_OBJECT
    // friend class OpenCV;

public:
    QtWindow(QWidget* parent = nullptr);
    // ~MainWindow() override;

private slots:
    void onEditButtonClicked();
    void onDebugButtonClicked();

private:
    // QWidget* centralWidget;
    void setupUi();
    void quitApp();
    GlobalResources globalResources; // Add PathsManager as a member variable
    // std::array<std::array<std::string, 20>, 20> img_paths;

};