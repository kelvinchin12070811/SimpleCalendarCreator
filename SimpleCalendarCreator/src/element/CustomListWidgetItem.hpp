/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>

#include <qpointer.h>
#include <qlistwidget.h>
#include <qgraphicsitem.h>

#include "element/Element.hpp"
#include "window/SimpleCalendarCreator.hpp"

/**
 * @brief Extended QListWidgetItem for storing calendar element.
 */
class CustomListWidgetItem : public QListWidgetItem
{
public:
    /**
     * Create ListWidgetItem.
     * @param mainWindow Reference to main window for rendering design outline, can't be nullptr.
     * @param label Text to show on the item.
     * @param object Calendar element that will be hold by the item. nullptr for not holding any element.
     */
    explicit CustomListWidgetItem(QPointer<SimpleCalendarCreator> mainWindow, const QString& label,
        std::unique_ptr<element::Element> object = nullptr);
    ~CustomListWidgetItem() noexcept;

    void setElement(std::unique_ptr<element::Element> value) noexcept;

    element::Element* getElement() noexcept;
    const element::Element* getElement() const noexcept;

    QGraphicsPixmapItem* getPixmapItem() noexcept;
    const QGraphicsPixmapItem* getPixmapItem() const noexcept;

    /**
     * Render the outline of this item to the outline window.
     */
    void renderOutline();
private:
    /**
     * @internal
     * Reference to main window for rendering design outline, can't be nullptr.
     */
    QPointer<SimpleCalendarCreator> mainWindow{ nullptr };
    /**
     * @internal
     * Reference to scene that hold the graphics item.
     */
    QPointer<QGraphicsScene> itemScene{ nullptr };

    /**
     * [read, write] Calendar element that hold by the item, nullptr for not holding any of it.
     */
    std::unique_ptr<element::Element> object{ nullptr };
    /**
     * [read] Id of PixmapItem that will be drawn on the outline window.
     * Memory of pixmapItem will be freed at the end if it does not own a parent.
     */
    QGraphicsPixmapItem* pixmapItem{ nullptr };
};