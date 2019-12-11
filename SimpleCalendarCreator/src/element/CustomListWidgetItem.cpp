/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "CustomListWidgetItem.hpp"
#include "SimpleCalendarCreator.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif // _DEBUG

CustomListWidgetItem::CustomListWidgetItem(const QString& label, std::unique_ptr<element::Element> object):
	QListWidgetItem(label), object(std::move(object))
{
	this->object->setParent(this);
}

void CustomListWidgetItem::setElement(std::unique_ptr<element::Element> value)
{
	this->object = std::move(value);
	this->object->setParent(this);
}

element::Element* CustomListWidgetItem::getElement()
{
	return object.get();
}

void CustomListWidgetItem::renderOutline()
{
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
