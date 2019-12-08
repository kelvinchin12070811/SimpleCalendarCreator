/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>
#include <qlistwidget.h>
#include "Element.hpp"

class CustomListWidgetItem : public QListWidgetItem
{
public:
	explicit CustomListWidgetItem(const QString& label, std::unique_ptr<element::Element> element = nullptr);
	void setElement(std::unique_ptr<element::Element> value);
	const element::Element* getElement() const;
private:
	std::unique_ptr<element::Element> element{ nullptr };
};