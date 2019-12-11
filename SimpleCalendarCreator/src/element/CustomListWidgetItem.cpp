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

CustomListWidgetItem::CustomListWidgetItem(const QString& label, std::unique_ptr<element::Element> object):
	QListWidgetItem(label), object(std::move(object))
{
	this->object->setParent(this);
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

element::Element* CustomListWidgetItem::getElement()
{
	return const_cast<element::Element*>(static_cast<const CustomListWidgetItem*>(this)->getElement());
}

const element::Element* CustomListWidgetItem::getElement() const
{
	return object.get();
}

void CustomListWidgetItem::renderOutline()
{
	if (object == nullptr) return;

	auto parent = dynamic_cast<SimpleCalendarCreator*>(
		this->listWidget()->parentWidget()->parentWidget()->parentWidget());
	assert(parent != nullptr);
	auto winOutline = parent->getUi()->winOutline;
	auto scene = winOutline->scene();
	if (pixmapItem != nullptr)
	{
		scene->removeItem(pixmapItem);
		delete pixmapItem;
	}
	if (scene == nullptr)
	{
		scene = new QGraphicsScene;
		winOutline->setScene(scene);
	}
	pixmapItem = new QGraphicsPixmapItem{ object->getRenderedGraphics() };
	scene->addItem(pixmapItem);
}
