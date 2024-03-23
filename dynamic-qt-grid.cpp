#include "dynamic-qt-grid.h"
#include "global-resources.h"
// #include "id-button.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>

#include <algorithm>
#include <iostream>

DynamicQtGrid::DynamicQtGrid(QWidget* parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);
    recreateGrid();
};


void DynamicQtGrid::recreateGrid() {

    clearLayout(gridLayout);

    QHBoxLayout* top_buttons_layout = new QHBoxLayout();
    // top_buttons_layout->setSpacing(0);
    QVBoxLayout* left_buttons_layout = new QVBoxLayout();
    QVBoxLayout* images_layout = new QVBoxLayout();
    //QVBoxLayout* right_buttons_layout = new QVBoxLayout(this);
    //right_buttons_layout->setSpacing(0);
    //QHBoxLayout* bottom_buttons_layout = new QHBoxLayout(this);
    //bottom_buttons_layout->setSpacing(0);

    gridLayout->addLayout(top_buttons_layout, 0, 1);
    gridLayout->addLayout(left_buttons_layout, 1, 0);
    gridLayout->addLayout(images_layout, 1, 1);
    //gridLayout->addLayout(right_buttons_layout, 1, 2);
    //gridLayout->addLayout(bottom_buttons_layout, 2, 1);



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
        top_buttons_layout->addWidget(mergeColumnButton);
        connect(mergeColumnButton, &QPushButton::clicked, this, [this, i]() {
            onMergeColumnButtonClicked(i);
            });
    }

    // Left Buttons Layout
    for (int i = 0; i < GlobalResources::num_of_rows; i++) {
        QPushButton* mergeRowButton = new QPushButton(this);
        mergeRowButton->setToolTip("Merge row");
        if (GlobalResources::merged_rows[i]) {
            mergeRowButton->setIcon(QIcon("images/icons/demerge-row.svg"));
        }
        else {
            mergeRowButton->setIcon(QIcon("images/icons/merge-row.svg"));
        }
        left_buttons_layout->addWidget(mergeRowButton);
        connect(mergeRowButton, &QPushButton::clicked, this, [this, i]() {
            onMergeRowButtonClicked(i);
            });
     }

    // Test and print if any row/col combined
    std::cout << "<<<<<< New Grid >>>>>>" << std::endl;
    std::cout << "GlobalResources::anyMergedCols(): " << GlobalResources::anyMergedCols() << std::endl;
    std::cout << "GlobalResources::anyMergedRows(): " << GlobalResources::anyMergedRows() << std::endl;


    // Images Layout


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
    DynamicQtGrid::recreateGrid();
}


void DynamicQtGrid::onMergeRowButtonClicked(int i) {
    GlobalResources::merged_rows[i] = !GlobalResources::merged_rows[i];
    DynamicQtGrid::recreateGrid();
}