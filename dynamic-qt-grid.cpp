#include "dynamic-qt-grid.h"
#include "global-resources.h"
#include "image-label.h"
// #include "qt-window.h"
// #include "id-button.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>

#include <algorithm>
#include <iostream>

DynamicQtGrid::DynamicQtGrid(QWidget* parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);
    // DynamicQtGrid::printLayoutChildren(gridLayout);
    recreateGrid();
};


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
    //right_buttons_layout->setSpacing(0);
    QHBoxLayout* bottomButtonsLayout = new QHBoxLayout();
    //bottom_buttons_layout->setSpacing(0);

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
            mergeColumnButton->setIcon(QIcon("images/icons/demerge-column.svg"));
        }
        else {
            mergeColumnButton->setIcon(QIcon("images/icons/merge-column.svg"));
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
            mergeRowButton->setIcon(QIcon("images/icons/demerge-row.svg"));
        }
        else {
            mergeRowButton->setIcon(QIcon("images/icons/merge-row.svg"));
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
                ImageLabel* i_label = new ImageLabel(0, r);
                insideImgLayout->addWidget(i_label);
            }
            else {      // current row not merged
                for (int c = 0; c < GlobalResources::num_of_cols; c++) {
                    ImageLabel* i_label = new ImageLabel(c, r);
                    insideImgLayout->addWidget(i_label);
                }
            }
        }
    }
    else {     // if we have combined columns
        for (int c = 0; c < GlobalResources::num_of_cols; c++) {
            QVBoxLayout* insideImgLayout = new QVBoxLayout();
            imagesLayout->addLayout(insideImgLayout);
            if (GlobalResources::merged_cols[c]) {      // current column merged
                ImageLabel* i_label = new ImageLabel(c, 0);
                insideImgLayout->addWidget(i_label);
            }
            else {      // current column not merged
                for (int r = 0; r < GlobalResources::num_of_rows; r++) {
                    ImageLabel* i_label = new ImageLabel(c, r);
                    insideImgLayout->addWidget(i_label);
                }
            }
        }

    }


    // Right Buttons Layout
    // hide column button
    QPushButton* hide_col_button = new QPushButton();
    hide_col_button->setToolTip("Remove column");
    hide_col_button->setIcon(QIcon("images/icons/hide-column.svg"));
    hide_col_button->setIconSize(QSize(20, 20));
    hide_col_button->setFixedWidth(40);
    hide_col_button->setFixedHeight(40);
    hide_col_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(hide_col_button, &QPushButton::clicked, this, &DynamicQtGrid::hideColumn);
    // add column button
    QPushButton* add_col_button = new QPushButton();
    add_col_button->setToolTip("Add new column");
    add_col_button->setIcon(QIcon("images/icons/add-column.svg"));
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
    add_row_button->setIcon(QIcon("images/icons/add-row.svg"));
    add_row_button->setToolTip("Add new row");
    add_row_button->setIconSize(QSize(20, 20));
    add_row_button->setFixedHeight(40);
    connect(add_row_button, &QPushButton::clicked, this, &DynamicQtGrid::addRow);
    // hide row button
    QPushButton* hide_row_button = new QPushButton();
    hide_row_button->setIcon(QIcon("images/icons/hide-row.svg"));
    hide_row_button->setToolTip("Remove row");
    hide_row_button->setIconSize(QSize(20, 20));
    hide_row_button->setFixedHeight(40);
    hide_row_button->setFixedWidth(40);
    connect(hide_row_button, &QPushButton::clicked, this, &DynamicQtGrid::hideRow);
    // add to layout
    bottomButtonsLayout->addWidget(add_row_button);
    bottomButtonsLayout->addWidget(hide_row_button);


    this->window()->setFixedSize(150 * GlobalResources::num_of_cols + 180, 130 * GlobalResources::num_of_rows + 180 + 100);

    // Grid Size Adjustment
    /*window->resize(150 * GlobalResources::num_of_cols, 130 * GlobalResources::num_of_rows);*/


        //# Using setFixedSize : (width, height)
        //self.setFixedSize(150 * self.pyqt_num_of_cols + 180, 130 * self.pyqt_num_of_rows + 180 + 100)
        //QtCore.QTimer.singleShot(1, lambda: self.show_grid_images())

    // DynamicQtGrid::printLayoutChildren(gridLayout);
};


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

void DynamicQtGrid::hideColumn() {
    if (GlobalResources::num_of_cols > 1) {
        GlobalResources::num_of_cols--;
        std::cout << GlobalResources::num_of_cols << std::endl;
        DynamicQtGrid::recreateGrid();
    }
}

void DynamicQtGrid::addColumn() {
    if (GlobalResources::num_of_cols < 20) {
        GlobalResources::num_of_cols++;
        std::cout << GlobalResources::num_of_cols << std::endl;
        DynamicQtGrid::recreateGrid();
    }
}

void DynamicQtGrid::hideRow() {
    if (GlobalResources::num_of_rows > 1) {
        GlobalResources::num_of_rows--;
        std::cout << GlobalResources::num_of_rows << std::endl;
        DynamicQtGrid::recreateGrid();
    }
}

void DynamicQtGrid::addRow() {
    if (GlobalResources::num_of_rows < 20) {
        GlobalResources::num_of_rows++;
        std::cout << GlobalResources::num_of_rows << std::endl;
        DynamicQtGrid::recreateGrid();
    }
}
// To Do: Calculate number of all elements in main window, check for memory safety
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