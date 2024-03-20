#include "dynamic-qt-grid.h"
#include "global-resources.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>

DynamicQtGrid::DynamicQtGrid(QWidget* parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);
    recreateGrid();
};


void DynamicQtGrid::recreateGrid() {

    clearLayout(gridLayout);

    QHBoxLayout* top_buttons_layout = new QHBoxLayout();
    top_buttons_layout->setSpacing(0);
    //QVBoxLayout* left_buttons_layout = new QVBoxLayout(this);
    //QVBoxLayout* images_layout = new QVBoxLayout(this);
    //QVBoxLayout* right_buttons_layout = new QVBoxLayout(this);
    //right_buttons_layout->setSpacing(0);
    //QHBoxLayout* bottom_buttons_layout = new QHBoxLayout(this);
    //bottom_buttons_layout->setSpacing(0);

    gridLayout->addLayout(top_buttons_layout, 0, 1);
    //gridLayout->addLayout(left_buttons_layout, 1, 0);
    //gridLayout->addLayout(images_layout, 1, 1);
    //gridLayout->addLayout(right_buttons_layout, 1, 2);
    //gridLayout->addLayout(bottom_buttons_layout, 2, 1);



    // Top Buttons Layout
    for (int i = 0; i < GlobalResources::num_of_cols; i++) {
        QPushButton* button = new QPushButton(this);
        button->setToolTip("Merge column");
        if (GlobalResources::combined_cols[i]) {
            button->setIcon(QIcon("images/icons/demerge-column.svg"));
        }
        else {
            button->setIcon(QIcon("images/icons/merge-column.svg"));
        }
        top_buttons_layout->addWidget(button);
        // connect(button, &QPushButton::clicked, [this]() { this->combineColumn(i); });
        //button1.clicked.connect(lambda state, x = c: self.combine_column(x))
    }






    // QVBoxLayout* main_layout = new QVBoxLayout(centralWidget);

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

void DynamicQtGrid::combineColumn(int i) {
    GlobalResources::combined_cols[i] = !GlobalResources::combined_cols[i];
}