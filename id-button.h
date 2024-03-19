#pragma once

#include <QPushButton>


class PositionedButton : public QPushButton {
	Q_OBJECT

public:
	PositionedButton(QWidget* parent = nullptr);

private:
	int position = -10;


};

