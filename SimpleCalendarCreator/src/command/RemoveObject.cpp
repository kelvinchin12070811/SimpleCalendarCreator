/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "command/RemoveObject.hpp"

#include <qgraphicsitem.h>
#include <qgraphicsscene.h>

#include "element/CustomListWidgetItem.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif

namespace command
{
    RemoveObject::RemoveObject(QListWidget* list, QGraphicsView* view):
        list(list), view(view), index(this->list->currentIndex())
    {
        assert(this->list != nullptr);  //list can't be nullptr.
        assert(this->view != nullptr);  //view can't be nullptr
    }

    bool RemoveObject::execute()
    {
        if (index.row() < 0) return false;
        item = decltype(item){ static_cast<CustomListWidgetItem*>(list->takeItem(index.row())) };
        QGraphicsScene* scene{ view->scene() };
        QGraphicsPixmapItem* pixmapItem{ item->getPixmapItem() };
        if (pixmapItem != nullptr)
            scene->removeItem(item->getPixmapItem());
        
#ifdef _DEBUG
        qDebug() << "item has parent: " << (item->listWidget() == nullptr ? "false" : "true");
#endif
        return true;
    }
    
    void RemoveObject::unexecute()
    {
        if (item == nullptr || index.row() < 0) return;
        QGraphicsScene* scene{ view->scene() };
        QGraphicsPixmapItem* pixmapItem{ item->getPixmapItem() };
        if (pixmapItem != nullptr)
            scene->addItem(pixmapItem);

        list->insertItem(index.row(), item.release());
    }
}
