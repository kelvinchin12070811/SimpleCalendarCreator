/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>
#include <QtWidgets/QMainWindow>
#include "ui_SimpleCalendarCreator.h"

class SimpleCalendarCreator : public QMainWindow
{
	Q_OBJECT

public:
	SimpleCalendarCreator(QWidget *parent = Q_NULLPTR);

private:
	void connectObjects();
	void onAddObject();

private:
	std::unique_ptr<Ui::SimpleCalendarCreatorClass> ui{ nullptr };
};
