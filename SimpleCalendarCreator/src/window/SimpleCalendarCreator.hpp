/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include "ui_SimpleCalendarCreator.h"

#include <memory>
#include <stack>

#include <QtWidgets/QMainWindow>

#include "command/Command.hpp"

class SimpleCalendarCreator : public QMainWindow
{
	Q_OBJECT

public:
	SimpleCalendarCreator(QWidget *parent = Q_NULLPTR);
	const Ui::SimpleCalendarCreatorClass* getUi() const;

private:
	void connectObjects();
	void initUi();

private: //slots
	void onActionUndo();
	void onAddObject();
	void onRemoveObject();

private:
	std::stack<std::unique_ptr<command::Command>> undoStack;
	std::unique_ptr<Ui::SimpleCalendarCreatorClass> ui{ nullptr };
};
