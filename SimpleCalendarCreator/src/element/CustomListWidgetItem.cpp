/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/SimpleCalendarCreator.hpp"

#include "element/CustomListWidgetItem.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif // _DEBUG

CustomListWidgetItem::CustomListWidgetItem(SimpleCalendarCreator* mainWindow, const QString& label,
    std::unique_ptr<element::Element> object):
        mainWindow(mainWindow), QListWidgetItem(label), object(std::move(object))
{
    this->object->setParent(this);
    this->object->setSize(this->mainWindow->getCalendarSize());
}

CustomListWidgetItem::~CustomListWidgetItem() noexcept
{
    if (pixmapItem != nullptr && pixmapItem->parentWidget() == nullptr)
        delete pixmapItem;
}

void CustomListWidgetItem::setElement(std::unique_ptr<element::Element> value) noexcept
{
    this->object = std::move(value);
    this->object->setParent(this);
}

element::Element* CustomListWidgetItem::getElement() noexcept
{
    return const_cast<element::Element*>(static_cast<const CustomListWidgetItem*>(this)->getElement());
}

const element::Element* CustomListWidgetItem::getElement() const noexcept
{
    return object.get();
}

QGraphicsPixmapItem* CustomListWidgetItem::getPixmapItem() noexcept
{
    return const_cast<QGraphicsPixmapItem*>(static_cast<const CustomListWidgetItem*>(this)->getPixmapItem());
}

const QGraphicsPixmapItem* CustomListWidgetItem::getPixmapItem() const noexcept
{
    return pixmapItem;
}

void CustomListWidgetItem::renderOutline()
{
    if (object == nullptr) return;

    QGraphicsView* winOutline{ mainWindow->getUi()->winOutline };
    QGraphicsScene* scene{ winOutline->scene() };
    if (pixmapItem != nullptr)
    {
        scene->removeItem(pixmapItem);
        delete pixmapItem;
    }
    pixmapItem = new QGraphicsPixmapItem{ object->getRenderedGraphics() };
    scene->addItem(pixmapItem);
}
