/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include <qmessagebox.h>
#include "SimpleCalendarCreator.hpp"

SimpleCalendarCreator::SimpleCalendarCreator(QWidget *parent)
	: QMainWindow(parent), ui(std::make_unique<Ui::SimpleCalendarCreatorClass>())
{
	ui->setupUi(this);
	connectObjects();
}

void SimpleCalendarCreator::connectObjects()
{
	connect(ui->actionAbout, &QAction::triggered,
		[this]() { QMessageBox::about(this, this->windowTitle(), "Created by Kelvin Chin"); });
	connect(ui->actionAbout_Qt, &QAction::triggered,
		[this]() { QMessageBox::aboutQt(this, this->windowTitle()); });
	connect(ui->actionQuit, &QAction::triggered, [this]() { this->close(); });
	connect(ui->btnAddObject, &QPushButton::clicked, this, &SimpleCalendarCreator::onAddObject);
}

void SimpleCalendarCreator::onAddObject()
{
	QMessageBox::information(this, this->windowTitle(), "clicked");
}
