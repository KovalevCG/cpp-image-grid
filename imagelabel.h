#pragma once

#include "globalresources.h"
#include <QLabel>
#include <QMimeData>

class ImageLabel : public QLabel {

	Q_OBJECT

public:
	explicit ImageLabel(QWidget* parent = nullptr);
	void setPosition(int col, int row);
	int getCol();
	int getRow();


private:
	int label_col = 0;
	int label_row = 0;
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
	GlobalResources globalResources;

};

