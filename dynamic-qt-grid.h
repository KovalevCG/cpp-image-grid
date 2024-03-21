#pragma once

#include "global-resources.h"

#include <QWidget>
#include <QGridLayout>


class DynamicQtGrid : public QWidget {
	Q_OBJECT

public:
	explicit DynamicQtGrid(QWidget* parent = nullptr);
	void recreateGrid();
	// void recreateQtGrid(int rows, int cols); // Method to recreate the layout

private:
	void clearLayout(QLayout* layout); // Utility method to clear the existing layout
	void onMergeColumnButtonClicked(int i);
	void onMergeRowButtonClicked(int i);
	QGridLayout* gridLayout;

	//private slots:
	//    void addRow();
	//    void addColumn();
	//    void deleteRow();
	//    void deleteColumn();

};
