#include "qt-window.h"
#include "opencv.h"
#include "image-label.h"
#include "dynamic-qt-grid.h"

#include <QRect>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QCoreApplication>
#include <QApplication>
#include <QFrame>
#include <QStyleFactory>

#include <iostream>
using namespace std;


QtWindow::QtWindow(QWidget* parent) : QMainWindow(parent) {

    setupUi();

    // globalResources.defaultFillImgPaths();

    //cout << "img_paths[0][0]: " << globalResources.getImagePath(0, 0) << endl;
    //cout << "img_paths[19][19]: " << globalResources.getImagePath(19, 19) << endl;
}

void QtWindow::setupUi() {


    // Window adjutments
    this->setWindowTitle("Image Grid");
    this->resize(350, 200);
    this->setWindowIcon(QIcon("./images/logo/Logo-V1.png"));
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    // Central Widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Menu
    QMenuBar* menuBar = this->menuBar();
    menuBar->setStyleSheet("QMenuBar { background-color: #f0f0f0; }");
    QMenu* fileMenu = menuBar->addMenu("File");
    QMenu* helpMenu = menuBar->addMenu("Help");
    fileMenu->addAction("Exit", this, &QtWindow::quitApp);

    // Line under menuBar
    QFrame* menu_line = new QFrame();
    menu_line->setFrameShape(QFrame::HLine);
    menu_line->setFrameShadow(QFrame::Sunken);

    // Image Labels
    //ImageLabel* ilabel1 = new ImageLabel();
    //ilabel1->setPosition(0, 0);
    //ImageLabel* ilabel2 = new ImageLabel();
    //ilabel2->setPosition(1, 0);

    DynamicQtGrid* dynamicQtGrid = new DynamicQtGrid;
    //dynamicQtGrid->recreateGrid();
    //QWidget* dynamicQtGrid;




    // Buttons
    QPushButton* editButton = new QPushButton("Edit");
    connect(editButton, &QPushButton::clicked, this, &QtWindow::onEditButtonClicked);
    QPushButton* debugButton = new QPushButton("Debug");
    connect(debugButton, &QPushButton::clicked, this, &QtWindow::onDebugButtonClicked);

    // Horizontal Layout
    //QHBoxLayout* grid_layout = new QHBoxLayout();
    //grid_layout->addWidget(ilabel1);
    //grid_layout->addWidget(ilabel2);

    // Vertical Layout
    QVBoxLayout* main_layout = new QVBoxLayout(centralWidget);
    main_layout->addWidget(menuBar);
    main_layout->addWidget(menu_line);
    //main_layout->addLayout(grid_layout);
    main_layout->addWidget(dynamicQtGrid);
    main_layout->addWidget(editButton);
    main_layout->addWidget(debugButton);

    // Set Layout
    centralWidget->setLayout(main_layout);

    //globalResources.setNumOfCols(2);
    //globalResources.setNumOfRows(1);

}

void QtWindow::onEditButtonClicked() {
    OpenCV opencv;
    opencv.displayOpenCVWindow();
}

void QtWindow::onDebugButtonClicked() {
    std::cout << "<<<<<< Debug >>>>>>" <<endl;
    cout << "img_paths[0][0]: " << GlobalResources::getImagePath(0, 0) << endl;
    cout << "img_paths[1][0]: " << GlobalResources::getImagePath(1, 0) << endl;

}

void QtWindow::quitApp() {
    QCoreApplication::quit();
}