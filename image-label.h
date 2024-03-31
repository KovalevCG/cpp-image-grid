#pragma once

#include "global-resources.h"

#include <QLabel>
#include <QMimeData>
#include <QPixmap>

class ImageLabel : public QLabel {
	Q_OBJECT

public:
	explicit ImageLabel(int col, int row, QWidget* parent = nullptr);
	// static void initStaticPixmap();
	int getCol();
	int getRow();
	void setImage();

private:
	//static QPixmap bg_qt_pixmap;

	int label_col;
	int label_row;

protected:
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

};
