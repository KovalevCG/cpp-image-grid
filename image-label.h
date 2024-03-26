#pragma once

#include "global-resources.h"

#include <QLabel>
#include <QMimeData>

class ImageLabel : public QLabel {
	Q_OBJECT

public:
	explicit ImageLabel(int col, int row, QWidget* parent = nullptr);
	// void setPosition(int col, int row);
	int getCol();
	int getRow();


private:
	int label_col;
	int label_row;
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
	void setImage();
	// GlobalResources globalResources;

};
