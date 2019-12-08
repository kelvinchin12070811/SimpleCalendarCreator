/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "AddObject.hpp"
#include "../window/ObjectCreator.hpp"

namespace command
{
	AddObject::AddObject(QListWidget* list) :
		list(list)
	{
		auto creator = std::make_unique<ObjectCreator>();
		creator->exec();
		if (!creator->isAccepted()) return;
		item = new CustomListWidgetItem{ creator->getObjectName(), creator->createElement() };
	}

	AddObject::~AddObject()
	{
		if (item != nullptr && item->listWidget() == nullptr)
			delete item;
	}

	bool AddObject::execute()
	{
		if (item == nullptr) return false;

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
