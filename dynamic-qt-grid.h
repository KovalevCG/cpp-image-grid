#pragma once

#include <QWidget>
#include <QGridLayout>


class DynamicQtGrid : public QWidget {
	Q_OBJECT

public:
	explicit DynamicQtGrid(QWidget* parent = nullptr);
	void recreateGrid();

private:
	void clearLayout(QLayout* layout);
	void onMergeColumnButtonClicked(int i);
	void onMergeRowButtonClicked(int i);
	void hideColumn();
	void addColumn();
	void hideRow();
	void addRow();
	void printLayoutChildren(QLayout* layout);
	void showGridImages();
	void hideMeForScreenshot(int scrn, int c, int r);
	QVBoxLayout* constructImageAndScreenshotLayout(int c, int r);

	QIcon demerge_column_icon;
	QIcon merge_column_icon;
	QIcon demerge_row_icon;
	QIcon merge_row_icon;
	QIcon add_column_icon;
	QIcon hide_column_icon;
	QIcon add_row_icon;
	QIcon hide_row_icon;
	QIcon edit_icon;
	QIcon monitor_1_icon;
	QIcon monitor_2_icon;
	QIcon monitor_3_icon;

	QGridLayout* gridLayout;

};
