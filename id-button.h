#pragma once

#include <QPushButton>


class IdButton : public QPushButton {
	Q_OBJECT

public:
	IdButton(QWidget* parent = nullptr);

private:
	int id = 0;


};
