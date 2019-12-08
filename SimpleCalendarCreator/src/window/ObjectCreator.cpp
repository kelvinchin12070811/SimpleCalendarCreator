/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "ObjectCreator.hpp"

ObjectCreator::ObjectCreator(QWidget *parent)
	: QDialog(parent), ui(std::make_unique<Ui::ObjectCreator>())
{
	ui->setupUi(this);
	connectObjects();
}

bool ObjectCreator::isAccepted()
{
	return accepted;
}

std::unique_ptr<element::Element> ObjectCreator::createElement()
{
	return nullptr;
}

QString ObjectCreator::getObjectName()
{
	return ui->lneObjectName->text();
}

void ObjectCreator::connectObjects()
{
	connect(ui->btnCancel, &QPushButton::clicked, this, &ObjectCreator::close);
	connect(ui->btnOk, &QPushButton::clicked, this, &ObjectCreator::onAccepted);
}

void ObjectCreator::onAccepted()
{
	accepted = true;
	this->close();
}
