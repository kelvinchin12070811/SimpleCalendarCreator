/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include <qmetaobject.h>
#include <qstringlist.h>
#include "ObjectCreator.hpp"
#include "../element/Line.hpp"

ObjectCreator::ObjectCreator(QWidget *parent)
	: QDialog(parent), ui(std::make_unique<Ui::ObjectCreator>())
{
	ui->setupUi(this);

	// Map elements readable name to class name
	objectFactory = {
		{ "Line", []() { return std::make_unique<element::Line>(); } }
	};

	initUi();
	connectObjects();
}

bool ObjectCreator::isAccepted()
{
	return accepted;
}

std::unique_ptr<element::Element> ObjectCreator::createElement()
{
	if (!isAccepted()) return nullptr;

	return objectFactory[ui->cmbObjectType->currentText()]();
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

void ObjectCreator::initUi()
{
	for (auto&& itr : objectFactory)
	{
		ui->cmbObjectType->addItem(itr.first);
	}
}

void ObjectCreator::onAccepted()
{
	accepted = true;
	this->close();
}
