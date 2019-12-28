/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <QColor>
#include <QRect>

#include "element/Element.hpp"

namespace element
{
    namespace object_properties
    {
        /**
         * @brief Defined as the properties of element::Rectangle
         */
        struct Rectangle
        {
            QRect rect;  /**< Rectangle definition. */
            QColor foregroundColour;  /**< Foreground colour of rectangle. */
            QColor backgroundColour;  /**< Background colour of rectangle. */
            int width;  /**< Line width of the rectangle. */
        };
    }
    /**
     * @brief Represented as Rectangle in Calendar object.
     */
    class Rectangle : public Element
    {
    public:
        /**
         * Construct new Rectangle object.
         */
        Rectangle();

        void setParent(CustomListWidgetItem* parent) override;
        void setSize(const QSize& size) override;
        const QPixmap& getRenderedGraphics() override;
        QPixmap render(const Month& month) override;
        void edit() override;
        void serialize(pugi::xml_node* node) override;
        void deserialize(const pugi::xml_node& node) override;

    private slots:
        /**
         * @internal
         * Slots when drawing for rectangle is required.
         */
        void drawRect();

    private:
        /**
         * @internal
         * Parent custom list widget item relay on.
         */
        CustomListWidgetItem* parent{ nullptr };
        /**
         * @internal
         * Properties of Rectangle.
         */
        object_properties::Rectangle properties;
        /**
         * @internal
         * Rendered outline buffer.
         */
        QPixmap graphic;
    };
}