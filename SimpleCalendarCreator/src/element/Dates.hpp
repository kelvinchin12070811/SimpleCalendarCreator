/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <tuple>
#include <vector>

#include <QColor>
#include <QFont>

#include "element/Element.hpp"

namespace element
{
    namespace object_properties
    {
        /**
         * @brief Properties of element::Date.
         */
        struct Dates
        {
            /** @brief Tuple elements of Dates::speacialDays. */
            struct SpeacialDaysIndex
            {
                enum{
                    group_name,  /**< Name of the group. */
                    group_colour,  /**< Colour of the group. */
                    group_members  /**< Dates to display. */
                };
            };
            uint8_t textAlign;  /**< Text alignment of label, Left: 1, Center: 2, Right: 3. */
            QColor weakdayColour;  /**< Colour of weakday labels' text. */
            QColor weakendColour;  /**< Colour of weakend labels' text. */
            QColor weakstartColour;  /**< Colour of weakstart labels' text. */
            QFont font;  /**< Font to draw text. */
            QRect drawArea;  /**< Area to render labels. */
            /**
             * Specail Days to mark on calendar, indexing with Dates::SpeacialDaysIndex.
             * Indexed as: Group name, group colour, array of name => date pair.
             */
            std::vector<std::tuple<QString, QString, std::vector<std::pair<QString, QString>>>> speacialDays;
        };

        /** Determine if two set of properties are equal. */
        inline bool operator==(const Dates& lhs, const Dates& rhs)
        {
            return (lhs.textAlign == rhs.textAlign) &&
                (lhs.weakdayColour == rhs.weakdayColour) &&
                (lhs.weakendColour == rhs.weakendColour) &&
                (lhs.weakstartColour == rhs.weakstartColour) &&
                (lhs.font == rhs.font) &&
                (lhs.drawArea == rhs.drawArea) &&
                (lhs.speacialDays == rhs.speacialDays);
        }

        /** Determine if two set of properties are not equal. */
        inline bool operator!=(const Dates& lhs, const Dates& rhs)
        {
            return !operator==(lhs, rhs);
        }
    }

    /**
     * @brief Dates of the calendar which tells what the date is.
     */
    class Dates : public Element
    {
    public:
        /** Background colour of outline bound in AARRGGBB format. */
        static constexpr char* const outline_bound_colour{ "#4c87ceeb" };
    public:
        /** Create new object with default properties. */
        Dates();

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
         * Render graphic for outline.
         */
        void drawOutline();
        /**
         * @internal
         * Generic rendering function.
         * @param painter Painter to draw with, must not be nullptr.
         * @param date Selected date to draw, used year and month only.
         */
        void drawLabels(QPainter* painter, const QDate& date);

    private:
        /**
         * @internal
         * Parent that holds the Dates object, must not be nullptr.
         */
        CustomListWidgetItem* parent{ nullptr };
        /**
         * @internal
         * Properties of Dates.
         */
        object_properties::Dates properties;
        /**
         * @internal
         * Rendered graphic for outline.
         */
        QPixmap graphic;
    };
}