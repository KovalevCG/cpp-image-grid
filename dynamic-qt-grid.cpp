#include "dynamic-qt-grid.h"
#include "global-resources.h"
#include "image-label.h"
#include "screenshot.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QTimer>
#include <QApplication>
#include <QScreen>
#include <QString>
#include <QMessageBox>

#include <iostream>
#include <string>

Screenshot screenshot_ocv;

//  INIT
DynamicQtGrid::DynamicQtGrid(QWidget* parent) : QWidget(parent) {
    // Main Grid Layout
    gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(5, 5, 5, 5);
    this->setLayout(gridLayout);

    // Icons
    demerge_column_icon = QIcon("images/icons/demerge-column.svg");
    merge_column_icon = QIcon("images/icons/merge-column.svg");
    demerge_row_icon = QIcon("images/icons/demerge-row.svg");
    merge_row_icon = QIcon("images/icons/merge-row.svg");
    add_column_icon = QIcon("images/icons/add-column.svg");
    hide_column_icon = QIcon("images/icons/hide-column.svg");
    add_row_icon = QIcon("images/icons/add-row.svg");
    hide_row_icon = QIcon("images/icons/hide-row.svg");
    edit_icon = QIcon("images/icons/edit.svg");
    monitor_1_icon = QIcon("images/monitor_1.png");
    monitor_2_icon = QIcon("images/monitor_2.png");
    monitor_3_icon = QIcon("images/monitor_3.png");

    recreateGrid();
};

//  RECREATE GRID
void DynamicQtGrid::recreateGrid() {

    clearLayout(gridLayout);

    QHBoxLayout* topButtonsLayout = new QHBoxLayout();
    QVBoxLayout* leftButtonsLayout = new QVBoxLayout();

    QBoxLayout* imagesLayout = nullptr;
    if (!GlobalResources::anyMergedCols()) {
        imagesLayout = new QVBoxLayout();
    }
    else {
        imagesLayout = new QHBoxLayout();
    }
    QVBoxLayout* rightButtonsLayout = new QVBoxLayout();
    QHBoxLayout* bottomButtonsLayout = new QHBoxLayout();

    gridLayout->setSpacing(5);

    gridLayout->addLayout(topButtonsLayout, 0, 1);
    gridLayout->addLayout(leftButtonsLayout, 1, 0);
    gridLayout->addLayout(imagesLayout, 1, 1);
    gridLayout->addLayout(rightButtonsLayout, 1, 2);
    gridLayout->addLayout(bottomButtonsLayout, 2, 1);


    // Top Buttons Layout
    for (int i = 0; i < GlobalResources::num_of_cols; i++) {
        QPushButton* mergeColumnButton = new QPushButton(this);
        mergeColumnButton->setToolTip("Merge column");
        if (GlobalResources::merged_cols[i]) {
            mergeColumnButton->setIcon(demerge_column_icon);
        }
        else {
            mergeColumnButton->setIcon(merge_column_icon);
        }
        topButtonsLayout->addWidget(mergeColumnButton);
        connect(mergeColumnButton, &QPushButton::clicked, this, [this, i]() { onMergeColumnButtonClicked(i); });
    }

    // Left Buttons Layout
    for (int i = 0; i < GlobalResources::num_of_rows; i++) {
        // create button
        QPushButton* mergeRowButton = new QPushButton(this);
        // setup button
        mergeRowButton->setToolTip("Merge row");
        if (GlobalResources::merged_rows[i]) {
            mergeRowButton->setIcon(demerge_row_icon);
        }
        else {
            mergeRowButton->setIcon(merge_row_icon);
        }
        mergeRowButton->setFixedWidth(25);
        mergeRowButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        // add button to layout
        leftButtonsLayout->addWidget(mergeRowButton);
        // connect button
        connect(mergeRowButton, &QPushButton::clicked, this, [this, i]() {
            onMergeRowButtonClicked(i);
            });
     }

    // Images Layout
    if (!GlobalResources::anyMergedCols()) {        // if we don't have combined columns
        for (int r = 0; r < GlobalResources::num_of_rows; r++) {
            QHBoxLayout* insideImgLayout = new QHBoxLayout();
            imagesLayout->addLayout(insideImgLayout);
            if (GlobalResources::merged_rows[r]) {      // current row merged
                insideImgLayout->addLayout(constructImageAndScreenshotLayout(0, r));
            }
            else {      // current row not merged
                for (int c = 0; c < GlobalResources::num_of_cols; c++) {
                    insideImgLayout->addLayout(constructImageAndScreenshotLayout(c, r));
                }
            }
        }
    }
    else {     // if we have combined columns
        for (int c = 0; c < GlobalResources::num_of_cols; c++) {
            QVBoxLayout* insideImgLayout = new QVBoxLayout();
            imagesLayout->addLayout(insideImgLayout);
            if (GlobalResources::merged_cols[c]) {      // current column merged
                insideImgLayout->addLayout(constructImageAndScreenshotLayout(c, 0));
            }
            else {      // current column not merged
                for (int r = 0; r < GlobalResources::num_of_rows; r++) {
                    insideImgLayout->addLayout(constructImageAndScreenshotLayout(c, r));
                }
            }
        }

    }

    // Right Buttons Layout
    // hide column button
    QPushButton* hide_col_button = new QPushButton();
    hide_col_button->setToolTip("Remove column");
    hide_col_button->setIcon(hide_column_icon);
    hide_col_button->setIconSize(QSize(20, 20));
    hide_col_button->setFixedWidth(40);
    hide_col_button->setFixedHeight(40);
    hide_col_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(hide_col_button, &QPushButton::clicked, this, &DynamicQtGrid::hideColumn);
    // add column button
    QPushButton* add_col_button = new QPushButton();
    add_col_button->setToolTip("Add new column");
    add_col_button->setIcon(add_column_icon);
    add_col_button->setIconSize(QSize(20, 20));
    add_col_button->setFixedWidth(40);
    add_col_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(add_col_button, &QPushButton::clicked, this, &DynamicQtGrid::addColumn);
    // add to layout
    rightButtonsLayout->addWidget(hide_col_button);
    rightButtonsLayout->addWidget(add_col_button);

    // Bottom Button Layout
    // add row button
    QPushButton* add_row_button = new QPushButton();
    add_row_button->setIcon(add_row_icon);
    add_row_button->setToolTip("Add new row");
    add_row_button->setIconSize(QSize(20, 20));
    add_row_button->setFixedHeight(40);
    connect(add_row_button, &QPushButton::clicked, this, &DynamicQtGrid::addRow);
    // hide row button
    QPushButton* hide_row_button = new QPushButton();
    hide_row_button->setIcon(hide_row_icon);
    hide_row_button->setToolTip("Remove row");
    hide_row_button->setIconSize(QSize(20, 20));
    hide_row_button->setFixedHeight(40);
    hide_row_button->setFixedWidth(40);
    connect(hide_row_button, &QPushButton::clicked, this, &DynamicQtGrid::hideRow);
    // add to layout
    bottomButtonsLayout->addWidget(add_row_button);
    bottomButtonsLayout->addWidget(hide_row_button);

    this->setFixedSize(140 * (GlobalResources::num_of_cols) + 170, 125 * (GlobalResources::num_of_rows) + 150);

    QTimer::singleShot(0, [this]() { 
    this->showGridImages();
    this->window()->adjustSize();
    this->window()->setFixedSize(this->window()->size()); 
    });
};

//  HIDE ME FOR SCREENSHOT
void DynamicQtGrid::hideMeForScreenshot(int scrn, int c, int r) {
    this->window()->hide();
    QTimer::singleShot(200, this, [this, scrn, c, r]() { this->makeScreenshot(scrn, c, r); });
    // screenshot.screenshotRegion(r, c);
}

//  MAKE SCREENSHOT
void DynamicQtGrid::makeScreenshot(int scrn, int c, int r) {
    auto screen = QApplication::screens()[scrn];
    QPixmap screenshot = screen->grabWindow(0);
    QString filename = QString("screenshots/screenshot_%1_%2.png").arg(c, 2, 10, QChar('0')).arg(r, 2, 10, QChar('0'));
    screenshot.save(filename, "PNG");

    if (screenshot_ocv.screenshotRegion(c, r)) { // Assuming screenshotRegion is a method that returns a bool
        this->show();
        GlobalResources::setImagePath(c, r, filename.toStdString());
        recreateGrid();
    }
    this->window()->show();

}

//  CONSTRUCT IMAGE + SCREENSHOT LAYOUT
QVBoxLayout* DynamicQtGrid::constructImageAndScreenshotLayout(int c, int r) {

    // Layouts
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(2);
    QHBoxLayout* screenshotLayout = new QHBoxLayout();
    screenshotLayout->setSpacing(0);

    // Construct Label
    ImageLabel* i_label = new ImageLabel(c, r);

    // Construct Screenshot Buttons
    QPushButton* screenBtn1 = new QPushButton("  Screenshot");
    screenBtn1->setToolTip("Screenshot of the first monitor");
    screenBtn1->setIcon(monitor_1_icon);

    QPushButton* screenBtn2 = new QPushButton();
    screenBtn2->setMaximumWidth(28);
    screenBtn2->setToolTip("Screenshot of second monitor");
    screenBtn2->setIcon(monitor_2_icon);

    QPushButton* screenBtn3 = new QPushButton();
    screenBtn3->setMaximumWidth(28);
    screenBtn3->setToolTip("Screenshot of third monitor");
    screenBtn3->setIcon(monitor_3_icon);

    // Check GlobalResources::WIN_VERSION
    if (GlobalResources::WIN_VERSION != 10) {
        screenBtn1->setEnabled(false);
        screenBtn1->setToolTip("Currently works on Windows 10 only");
        screenBtn2->setEnabled(false);
        screenBtn2->setToolTip("Currently works on Windows 10 only");
        screenBtn3->setEnabled(false);
        screenBtn3->setToolTip("Currently works on Windows 10 only");
    }

    // Add Items to Layouts
    screenshotLayout->addWidget(screenBtn1);
    if (QApplication::screens().size() > 1) {
        screenshotLayout->addWidget(screenBtn2);
    }
    if (QApplication::screens().size() > 2) {
        screenshotLayout->addWidget(screenBtn3);
    }
    layout->addWidget(i_label);
    layout->addLayout(screenshotLayout);

    // Connect Screenshots Buttons
    QObject::connect(screenBtn1, &QPushButton::clicked, [this, c, r]() { this->hideMeForScreenshot(0, c, r); });
    QObject::connect(screenBtn2, &QPushButton::clicked, [this, c, r]() { this->hideMeForScreenshot(1, c, r); });
    QObject::connect(screenBtn3, &QPushButton::clicked, [this, c, r]() { this->hideMeForScreenshot(2, c, r); });

    return layout;
}

//  SHOW GRID IMAGES
void DynamicQtGrid::showGridImages() {
    // Find all child ImageLabel instances
    const auto imageLabels = findChildren<ImageLabel*>();

    // Iterate through the list and set the image for each ImageLabel
    for (ImageLabel* label : imageLabels) {
        if (label) { // Check to make sure the pointer is valid
            label->setImage();
        }
    }
}

//  CLEAR LAYOUT
void DynamicQtGrid::clearLayout(QLayout* layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        // Delete any widgets in the layout item
        if (QWidget* widget = item->widget()) {
            delete widget;
        }
        else if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout); // Recursively clear
        }
        delete item;
    }
}

// ON MERGE BUTTON CLICKED
void DynamicQtGrid::onMergeColumnButtonClicked(int i) {
    GlobalResources::merged_cols[i] = !GlobalResources::merged_cols[i];
    // Fill all rows with "false"
    std::fill(GlobalResources::merged_rows.begin(), GlobalResources::merged_rows.end(), false);
    DynamicQtGrid::recreateGrid();
}
void DynamicQtGrid::onMergeRowButtonClicked(int i) {
    GlobalResources::merged_rows[i] = !GlobalResources::merged_rows[i];
    // Fill all colls with "false"
    std::fill(GlobalResources::merged_cols.begin(), GlobalResources::merged_cols.end(), false);
    DynamicQtGrid::recreateGrid();
}

//  ADD/HIDE COLUMN/ROW
void DynamicQtGrid::hideColumn() {
    if (GlobalResources::num_of_cols > 1) {
        GlobalResources::num_of_cols--;
        DynamicQtGrid::recreateGrid();
    }
}
void DynamicQtGrid::addColumn() {
    if (GlobalResources::num_of_cols < GlobalResources::SIZE) {
        GlobalResources::num_of_cols++;
        DynamicQtGrid::recreateGrid();
    }
    else {
        freeVersionWindow();
    }
}
void DynamicQtGrid::hideRow() {
    if (GlobalResources::num_of_rows > 1) {
        GlobalResources::num_of_rows--;
        DynamicQtGrid::recreateGrid();
    }
}
void DynamicQtGrid::addRow() {
    if (GlobalResources::num_of_rows < GlobalResources::SIZE) {
        GlobalResources::num_of_rows++;
        DynamicQtGrid::recreateGrid();
    }
    else {
        freeVersionWindow();
    }
}

void DynamicQtGrid::freeVersionWindow() {
    QMessageBox freeVersion;
    freeVersion.setIcon(QMessageBox::Information);
    freeVersion.setWindowTitle("Free Version");
    freeVersion.setText("The free version of ImageGrid\nis limited to 2 columns and 2\nrows.");
    freeVersion.setWindowFlags(Qt::WindowStaysOnTopHint);
    freeVersion.exec();
}

//  TEST METHOD.  PRINT LAYOUT CHILDREN       To Do: Calculate number of all elements in main window, check for memory safety, better representation on screen
void DynamicQtGrid::printLayoutChildren(QLayout* layout) {
    std::cout << "----- Start of Layout Children Print ------" << "; layout->count(): " << layout->count() << std::endl;
    if (!layout) {
        std::cout << "The layout is null!" << std::endl;
        return;
    }

    for (int i = 0; i < layout->count(); ++i) {

        QLayoutItem* item = layout->itemAt(i);

        if (item->widget()) {
            // This item is a widget
            QWidget* widget = item->widget();
            std::cout << "Widget name:" << widget->objectName().toStdString() << ", Widget class:" << widget->metaObject()->className() << std::endl;
        }
        else if (item->layout()) {
            // This item is a nested layout. You could recursively call printLayoutChildren here if needed.
            std::cout << "Found a nested layout; " << std::endl;
            DynamicQtGrid::printLayoutChildren(item->layout());
        }
        else if (item->spacerItem()) {
            // This item is a spacer item.
            std::cout << "Found a spacer item; " << std::endl;
        }
    }
}