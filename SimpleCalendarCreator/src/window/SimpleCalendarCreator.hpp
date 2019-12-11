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

/**
 * @brief Main window of the program. Initialized by the main entry point.
 */
class SimpleCalendarCreator : public QMainWindow
{
	Q_OBJECT

public:
	SimpleCalendarCreator(QWidget *parent = Q_NULLPTR);
	/**
	 * @brief Get ui elements of the MainWindow.
	 */
	const Ui::SimpleCalendarCreatorClass* getUi() const noexcept;

private:
	void connectObjects();
	void initUi();

private: //slots
	/**
	 * @internal
	 * @brief Slot when undo action is trigred.
	 */
	void onActionUndo();
	/**
	 * @internal
	 * @brief Slot when a calendar object is being to add to the design.
	 */
	void onAddObject();
	/**
	 * @internal
	 * @brief Slot when a calendar object is being to remove from the design.
	 */
	void onRemoveObject();

private:
	/**
	 * @internal
	 * @brief History tracking stack that holds previous executed command.
	 */
	std::stack<std::unique_ptr<command::Command>> undoStack;
	/**
	 * @internal
	 * @brief Ui elements of the window.
	 */
	std::unique_ptr<Ui::SimpleCalendarCreatorClass> ui{ nullptr };
};
