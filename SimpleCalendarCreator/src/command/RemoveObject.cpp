/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "command/RemoveObject.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif

namespace command
{
    RemoveObject::RemoveObject(QListWidget* list):
        list(list), index(this->list->currentIndex())
    {
    }

    bool RemoveObject::execute()
    {
        if (index.row() < 0) return false;
        item = decltype(item){ list->takeItem(index.row()) };
#ifdef _DEBUG
        qDebug() << "item has parent: " << (item->listWidget() == nullptr ? "false" : "true");
#endif
        return true;
    }
    
    void RemoveObject::unexecute()
    {
        if (item == nullptr || index.row() < 0) return;
        list->insertItem(index.row(), item.release());
    }
}
