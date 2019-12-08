/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "AddObject.hpp"

namespace command
{
	AddObject::AddObject(QListWidget& list, const QString& text) :
		list(&list)
	{
		item = new QListWidgetItem{ text };
	}

	AddObject::~AddObject()
	{
		if (item->listWidget() == nullptr)
			delete item;
	}

	bool AddObject::execute()
	{
		list->addItem(item);
		return true;
	}

	void AddObject::unexecute()
	{
		auto idx = list->row(item);
		if (idx < 0) return;
		list->takeItem(idx);
	}
}
