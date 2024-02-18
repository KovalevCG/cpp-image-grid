#include "dynamic-qt-grid.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

DynamicQtGrid::DynamicQtGrid(QWidget* parent) : QWidget(parent) {
	gridLayout = new QGridLayout(this);

};


void DynamicQtGrid::recreateGrid() {

    clearLayout(gridLayout);

    //# New Image Grid
    //self.main_grid_layout = QtWidgets.QGridLayout() +

    //self.top_buttons_layout = QtWidgets.QHBoxLayout() +
    //self.left_buttons_layout = QtWidgets.QVBoxLayout() +
    //self.images_layout = QtWidgets.QVBoxLayout() +
    //self.right_button_layout = QtWidgets.QVBoxLayout() +
    //self.right_button_layout.setSpacing(0) +
    //self.bottom_button_layout = QtWidgets.QHBoxLayout() +
    //self.bottom_button_layout.setSpacing(0) +


    //# Main Layout
    //self.main_grid_layout.addLayout(self.top_buttons_layout, 0, 1) +
    //self.main_grid_layout.addLayout(self.left_buttons_layout, 1, 0) +
    //self.main_grid_layout.addLayout(self.images_layout, 1, 1) +
    //self.main_grid_layout.addLayout(self.right_button_layout, 1, 2) +
    //self.main_grid_layout.addLayout(self.bottom_button_layout, 2, 1)

    //self.construct_grid()

    QHBoxLayout* top_buttons_layout = new QHBoxLayout(this);
    QVBoxLayout* left_buttons_layout = new QVBoxLayout(this);
    QVBoxLayout* images_layout = new QVBoxLayout(this);
    QVBoxLayout* right_buttons_layout = new QVBoxLayout(this);
    right_buttons_layout->setSpacing(0);
    QHBoxLayout* bottom_buttons_layout = new QHBoxLayout(this);
    bottom_buttons_layout->setSpacing(0);

    gridLayout->addLayout(top_buttons_layout, 0, 1);
    gridLayout->addLayout(left_buttons_layout, 1, 0);
    gridLayout->addLayout(images_layout, 1, 1);
    gridLayout->addLayout(right_buttons_layout, 1, 2);
    gridLayout->addLayout(bottom_buttons_layout, 2, 1);




    for (int i = 0; i <= GlobalResources::num_of_cols; i++) {
        QPushButton* button = new QPushButton(this);
        button->setToolTip("Merge column");
    }


    //if combined_cols[c] == 1:
    //  button1.setIcon(QtGui.QIcon("./img/SVG/demerge-column.svg"))
    //else:
    //  button1.setIcon(QtGui.QIcon("./img/SVG/merge-column.svg"))
    //button1.clicked.connect(lambda state, x = c: self.combine_column(x))
    //elem_layout.addWidget(button1)
    //elem_layout.setSpacing(0)
    //self.top_buttons_layout.addLayout(elem_layout)



	// QVBoxLayout* main_layout = new QVBoxLayout(centralWidget);

};

void DynamicQtGrid::clearLayout(QLayout* layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            delete widget;
        }
        else if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout); // Recursively clear
            delete childLayout;
        }
        delete item;
    }
}
