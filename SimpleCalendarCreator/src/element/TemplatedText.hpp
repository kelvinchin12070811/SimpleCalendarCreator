/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <QFont>
#include <QString>
#include <QStringList>

#include "element/Element.hpp"

namespace element
{
    namespace object_properties
    {
        /**
         * @brief Properties of element::TemplatedText.
         */
        struct TemplatedText
        {
            bool isVertical;  /**< Determine if the text is vertical aligned. */
            uint8_t textAlign; /**< Determin the alignment of text. Left: 1, Center: 2, Right: 3 */
            QColor textColour;  /**< Colour of the text. */
            QFont font;  /**< Font use to render the text. */
            QPoint pos;  /**< Position of the text. */
            QStringList texts;  /**< Texts that will be rendered on each month. */
        };

        /** Determine if two set of properties are equal. */
        inline bool operator==(const TemplatedText& lhs, const TemplatedText& rhs)
        {
            return (lhs.isVertical == rhs.isVertical) &&
                (lhs.textAlign == rhs.textAlign) &&
                (lhs.textColour == rhs.textColour) &&
                (lhs.font == rhs.font) &&
                (lhs.pos == rhs.pos) &&
                (lhs.texts == rhs.texts);
        }

        /** Determine if two set of properties are not equal. */
        inline bool operator!=(const TemplatedText& lhs, const TemplatedText& rhs)
        {
            return !operator==(lhs, rhs);
        }
    }
    /**
     * @brief Text Object that render different text on each month.
     */
    class TemplatedText : public Element
    {
    public:
        /**
         * Create new TemplatedText object.
         */
        TemplatedText();

        void setParent(CustomListWidgetItem* parent) override;
        void setSize(const QSize& size) override;
        const QPixmap& getRenderedGraphics() override;
        QPixmap render(const QDate& date) override;
        void edit(QWidget* parent = nullptr) override;
        void serialize(pugi::xml_node* node) override;
        void deserialize(const pugi::xml_node& node) override;
        
    private:
        /**
         * @internal
         * Render to outline window.
         */
        void drawOutline();
        /**
         * @internal
         * General text rendering function.
         * @param painter Painter to draw on, must not be nullptr.
         * @param date Selected date to draw a month of that date.
         */
        void drawText(QPainter* painter, const QDate& date);

    private:
        /**
         * @internal
         * Parent of the Calendar Object, must not be nullptr.
         */
        CustomListWidgetItem* parent{ nullptr };
        /**
         * @internal
         * Properties of the Calendar Object.
         */
        object_properties::TemplatedText properties;
        /**
         * @internal
         * Renderend graphics for outline.
         */
        QPixmap graphic;
    };
}