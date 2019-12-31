/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <vector>

#include <QColor>
#include <QFont>
#include <QRect>
#include <QString>

#include "element/Element.hpp"

namespace element
{
    namespace object_properties
    {
        /**
         * @brief Properties of element::WeakTitle
         */
        struct WeakTitle
        {
            bool isVertical;  /**< Determine if the text is vertically oriented, reserved. */
            uint8_t textAlignment;  /**< Alignment of the text where, Left: 1, Center: 2, Right: 3 */
            QColor normalTextColour;  /**< Colour of weakdays' label. */
            QColor satTextColour;  /**< Colour of satuaday label. */
            QColor sunTextColour;  /**< Colour of sunday label. */
            QFont font;  /**< Font to render the labels. */
            /**
             * Area to render weak label.
             */
            QRect fontRect;
            /**
             * Multidimentional array of strig as [month varient][weak days' labels].
             * Pair:= First: Name of label set, Second: Label set.
             */
            std::vector<std::pair<QString, std::vector<QString>>> lables;
        };

        /** Determine if two set of properties are equal. */
        inline bool operator==(const WeakTitle& lhs, const WeakTitle& rhs)
        {
            return (lhs.isVertical == rhs.isVertical) &&
                (lhs.textAlignment == rhs.textAlignment) &&
                (lhs.normalTextColour == rhs.normalTextColour) &&
                (lhs.satTextColour == rhs.satTextColour) &&
                (lhs.sunTextColour == rhs.sunTextColour) &&
                (lhs.font == rhs.font) &&
                (lhs.fontRect == rhs.fontRect) &&
                (lhs.lables == rhs.lables);
        }

        /** Determin if two set of properties are not equal. */
        inline bool operator!=(const WeakTitle& lhs, const WeakTitle& rhs)
        {
            return !operator==(lhs, rhs);
        }
    }

    class WeakTitle : public Element
    {
    public:
        /**
         * Colour to draw on outline where the title will be drawn.
         */
        static constexpr char* const outline_background_colour{ "#20ff0000" };
    public:
        /**
         * Construct new object with default values.
         */
        WeakTitle();

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
         * Render outline of the title.
         */
        void drawOutline();
        /**
         * @internal
         * General rendering function.
         */
        void drawTitle(QPainter* painter, const QDate& date);
    private:
        /**
         * @internal
         * Parent object which holds the object, must not be nullptr.
         */
        CustomListWidgetItem* parent{ nullptr };
        /**
         * @internal
         * Properties of WeakTitle.
         */
        object_properties::WeakTitle properties;
        /**
         * @internal
         * Rendered graphic for project outline.
         */
        QPixmap graphic;
    };
}