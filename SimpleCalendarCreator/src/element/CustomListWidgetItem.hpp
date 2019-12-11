/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>

#include <qlistwidget.h>
#include <qgraphicsitem.h>

#include "element/Element.hpp"

/**
 * @brief Exteded QListWidgetItem for storing calendar element.
 */
class CustomListWidgetItem : public QListWidgetItem
{
public:
	/**
	 * @brief Create ListWidgetItem.
	 * @param label Text to show on the item.
	 * @param object Calendar element that will be hold by the item. nullptr for not holding any element.
	 */
	explicit CustomListWidgetItem(const QString& label, std::unique_ptr<element::Element> object = nullptr);
	~CustomListWidgetItem() noexcept;

	void setElement(std::unique_ptr<element::Element> value) noexcept;
	element::Element* getElement();
	const element::Element* getElement() const;

	/**
	 * @brief Render the outline of this item to the outline window.
	 */
	void renderOutline();
private:
	/**
	 * @brief [read, write] Calendar element that hold by the item, nullptr for not holding any of it.
	 */
	std::unique_ptr<element::Element> object{ nullptr };
	/**
	 * @internal
	 * @brief Id of PixmapItem that will be drawn on the outline window.
	 * Memory of pixmapItem will be freed at the end if it does not own a parent.
	 */
	QGraphicsPixmapItem* pixmapItem{ nullptr };
};