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
            bool verticalText;  /**< Determine if the renderer should draw the text vertically. */
            uint8_t textAlignment;  /**< Determine the alignment of text. Left: 1, Center: 2, Right: 3*/
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
        /** Height of line. */
        static constexpr qreal line_height{ 70 };
        /** CSS style of the text to render. */
        static constexpr char* const css_rules{ "<span style='color: %2; text-align: center'>%1</span>" };
    public:
        /**
         * Create Text object
         */
        Text();

        void setParent(CustomListWidgetItem* parent) override;
        void setSize(const QSize& size) override;
        const QPixmap& getRenderedGraphics() override;
        QPixmap render(const QDate& date) override;
        void edit() override;
        void serialize(pugi::xml_node* node) override;
        void deserialize(const pugi::xml_node& node) override;
    private:
        /**
         * @internal
         * Render graphic for outline.
         */
        void drawOutline();
        /**
         * @internal
         * General text renderer.
         * @param painter Painter to draw on, must not be nullptr.
         */
        void drawText(QPainter* painter);
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