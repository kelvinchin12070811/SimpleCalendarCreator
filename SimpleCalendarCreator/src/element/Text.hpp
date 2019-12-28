/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <QFont>
#include <QPoint>
#include <QString>

#include "element/Element.hpp"

namespace element
{
    namespace object_properties
    {
        /**
         * @brief properties of calendar object element::Text.
         */
        struct Text
        {
            QColor textColour;  /**< Colour of the text. */
            QFont font;  /**< Font info. */
            QPoint pos;  /**< Text position. */
            QString text;  /**< Text to render. */
        };
    }
    /**
     * @brief Represented as Text on the calendar.
     */
    class Text : public Element
    {
    public:
        /**
         * Create Text object
         */
        Text();

        void setParent(CustomListWidgetItem* parent) override;
        void setSize(const QSize& size) override;
        const QPixmap& getRenderedGraphics() override;
        QPixmap render(const Month& month) override;
        void edit() override;
        void serialize(pugi::xml_node* node) override;
        void deserialize(const pugi::xml_node& node) override;
    private:
        void drawText();
    private:
        /**
         * @internal
         * Parent of the calendar object. Must not be nullptr.
         */
        CustomListWidgetItem* parent{ nullptr };
        /**
         * @internal
         * Properties of the Text object.
         */
        object_properties::Text properties;
        /**
         * @internal
         * Rendered graphic for previewing outline.
         */
        QPixmap graphic;
    };
}