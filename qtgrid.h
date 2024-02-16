#pragma once

#include "globalresources.h"

#include <QWidget>


class QtGrid : public QWidget {
	Q_OBJECT

public:
	explicit QtGrid(QWidget* parent = nullptr) {};

private:
	void createGridOfLabels();

//private slots:
//    void addRow();
//    void addColumn();
//    void deleteRow();
//    void deleteColumn();

};

