#include "qt-test.h"

#include <QVBoxLayout>
#include <QMessageBox>

QtTest::QtTest(QWidget* parent) : QMainWindow(parent), ocvTestWindow(new OcvTest()) {
    setupUi();
    ocvTestWindow->setAboutCallback([this]() { this->showAbout(); });
}

QtTest::~QtTest() {
    delete ocvTestWindow;
}

void QtTest::setupUi() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout;
    centralWidget->setLayout(layout);

    startButton = new QPushButton("Start OCV Test", this);
    aboutButton = new QPushButton("About QtTest", this);
    layout->addWidget(startButton);
    layout->addWidget(aboutButton);

    connect(startButton, &QPushButton::clicked, [this]() { ocvTestWindow->show(); });
    connect(aboutButton, &QPushButton::clicked, this, &QtTest::showAbout);

    setWindowTitle("Qt Test");
}

void QtTest::showAbout() {
    QMessageBox::about(this, "About QtTest", "QtTest Version 1.0");
}