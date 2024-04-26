#include "qt-window.h"
#include "opencv.h"
#include "ocv-test.h"
#include "dynamic-qt-grid.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QApplication>
#include <QFrame>
#include <QMessageBox>

#include <iostream>

using std::cout;
using std::endl;

OpenCV opencv;
OcvTest ocvTest;

QtWindow::QtWindow(QWidget* parent) : QMainWindow(parent) {

    setupUi();

}

void QtWindow::setupUi() {

    // Window adjutments
    this->setWindowTitle("Image Grid v" + QString::fromStdString(GlobalResources::VERSION));
    this->setWindowIcon(QIcon("./images/logo/Logo-V1.png"));
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    // Central Widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Menu
    QMenuBar* menuBar = this->menuBar();
    menuBar->setStyleSheet("QMenuBar { background-color: #f0f0f0; }");
    QMenu* fileMenu = menuBar->addMenu("File");
    QMenu* helpMenu = menuBar->addMenu("Help");
    fileMenu->addAction("Exit", this, &QtWindow::quitApp);
    fileMenu->addAction("Run OCV Test", this, &QtWindow::runOcvTest);
    helpMenu->addAction("About", this, &QtWindow::showAbout);

    // Line under menuBar
    QFrame* menuLine = new QFrame();
    menuLine->setFrameShape(QFrame::HLine);
    menuLine->setFrameShadow(QFrame::Sunken);

    // Dynamic Grid
    DynamicQtGrid* dynamicQtGrid = new DynamicQtGrid;

    // Line under menuBar
    QFrame* editLine = new QFrame();
    editLine->setFrameShape(QFrame::HLine);
    editLine->setFrameShadow(QFrame::Sunken);

    // Buttons
    QPushButton* editButton = new QPushButton("Edit");
    editButton->setFixedHeight(50);
    editButton->setIcon(QIcon("images/icons/edit.svg"));
    editButton->setIconSize(QSize(20, 20));
    editButton->setToolTip("Open Editor");
    connect(editButton, &QPushButton::clicked, this, &QtWindow::onEditButtonClicked);

    // Vertical Layout
    mainLayout->addWidget(menuBar);
    mainLayout->addWidget(menuLine);
    mainLayout->addWidget(dynamicQtGrid);
    mainLayout->addWidget(editLine);
    mainLayout->addWidget(editButton);

    // Set Layout
    centralWidget->setLayout(mainLayout);
}

void QtWindow::onEditButtonClicked() {
    opencv.startOpencvMainLoop();
}

void QtWindow::quitApp() {
    QCoreApplication::quit();
}

void QtWindow::runOcvTest() {
    ocvTest.runTest();
}

void QtWindow::showAbout() {
    QMessageBox aboutBox;
    aboutBox.setIcon(QMessageBox::Information);
    aboutBox.setWindowTitle("About");
    aboutBox.setText("  Image Grid v" + QString::fromStdString(GlobalResources::VERSION) + "   \n\n           2024         \n by Alexander Kovalev \n kovalev.cg@gmail.com");
    aboutBox.setWindowFlags(Qt::WindowStaysOnTopHint);
    aboutBox.exec();
}