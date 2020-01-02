/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "command/AddObject.hpp"

#include "window/ObjectCreator.hpp"

namespace command
{
    AddObject::AddObject(SimpleCalendarCreator* mainWindow, QListWidget* list) :
        list(list)
    {
        auto creator = std::make_unique<ObjectCreator>(mainWindow);
        creator->exec();
        if (!creator->isAccepted()) return;
        item = new CustomListWidgetItem{ mainWindow, creator->getObjectName(), creator->createElement() };
    }

    AddObject::~AddObject() noexcept
    {
        if (item != nullptr && item->listWidget() == nullptr)
            delete item;
    }

    bool AddObject::execute()
    {
        if (item == nullptr) return false;

        list->addItem(item);
        item->getElement()->edit();
        return true;
    }

    void AddObject::unexecute()
    {
        auto idx = list->row(item);
        if (idx < 0) return;
        list->takeItem(idx);
    }
}
