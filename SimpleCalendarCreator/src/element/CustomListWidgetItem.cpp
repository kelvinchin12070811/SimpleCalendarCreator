/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "CustomListWidgetItem.hpp"

CustomListWidgetItem::CustomListWidgetItem(const QString& label, std::unique_ptr<element::Element> element):
	QListWidgetItem(label), element(std::move(element))
{
}

void CustomListWidgetItem::setElement(std::unique_ptr<element::Element> value)
{
	this->element = std::move(value);
}

const element::Element* CustomListWidgetItem::getElement() const
{
	return element.get();
}
