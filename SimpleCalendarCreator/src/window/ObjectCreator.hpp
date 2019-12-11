/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include "element/Element.hpp"

#include <functional>
#include <map>
#include <memory>

#include <qdialog.h>

#include "ui_ObjectCreator.h"

class ObjectCreator : public QDialog
{
	Q_OBJECT

public:
	ObjectCreator(QWidget *parent = Q_NULLPTR);
	~ObjectCreator() = default;

	bool isAccepted();
	std::unique_ptr<element::Element> createElement();
	QString getObjectName();

private:
	void connectObjects();
	void initUi();

private: // slots
	void onAccepted();

private:
	bool accepted{ false };
	std::map<QString, std::function<std::unique_ptr<element::Element>()>> objectFactory;
	std::unique_ptr<Ui::ObjectCreator> ui{ nullptr };
};
