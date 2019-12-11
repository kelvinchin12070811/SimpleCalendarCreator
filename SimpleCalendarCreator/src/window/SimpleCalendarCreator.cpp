/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/SimpleCalendarCreator.hpp"

#include <qdatetime.h>
#include <qmessagebox.h>

#include "command/AddObject.hpp"
#include "command/RemoveObject.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif

SimpleCalendarCreator::SimpleCalendarCreator(QWidget *parent)
	: QMainWindow(parent), ui(std::make_unique<Ui::SimpleCalendarCreatorClass>())
{
	ui->setupUi(this);
	connectObjects();
	initUi();
}

const Ui::SimpleCalendarCreatorClass* SimpleCalendarCreator::getUi() const noexcept
{
	return ui.get();
}

void SimpleCalendarCreator::connectObjects()
{
	connect(ui->actionAbout, &QAction::triggered,
		[this]() { QMessageBox::about(this, this->windowTitle(), "Created by Kelvin Chin"); });
	connect(ui->actionAbout_Qt, &QAction::triggered,
		[this]() { QMessageBox::aboutQt(this, this->windowTitle()); });
	connect(ui->actionQuit, &QAction::triggered, [this]() { this->close(); });
	connect(ui->actionUndo, &QAction::triggered, this, &SimpleCalendarCreator::onActionUndo);
	connect(ui->btnAddObject, &QPushButton::clicked, this, &SimpleCalendarCreator::onAddObject);
	connect(ui->btnEditObject, &QPushButton::clicked, [this]() {
		auto itm = this->ui->objectList->currentItem();
		if (itm == nullptr) return;
		static_cast<CustomListWidgetItem*>(itm)->getElement()->edit();
	});
	connect(ui->btnRemoveObject, &QPushButton::clicked, this, &SimpleCalendarCreator::onRemoveObject);
}

void SimpleCalendarCreator::initUi()
{
	QDate today{ QDate::currentDate() };
	ui->spnYear->setValue(today.year());
}

void SimpleCalendarCreator::onActionUndo()
{
	if (undoStack.empty()) return;
	undoStack.top()->unexecute();
	undoStack.pop();
}

void SimpleCalendarCreator::onAddObject()
{
	auto act = std::make_unique<command::AddObject>(this, ui->objectList);
	if (!act->execute()) return;
	undoStack.push(std::move(act));
}

void SimpleCalendarCreator::onRemoveObject()
{
	auto act = std::make_unique<command::RemoveObject>(ui->objectList);
	if (!act->execute()) return;
	undoStack.push(std::move(act));
}
