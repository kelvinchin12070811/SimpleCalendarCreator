/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <array>
#include <QFont>
#include <QLocale>

#include "element/Element.hpp"

namespace element
{
    namespace object_properties
    {
        /**
         * @brief Properties of element::MonthTitle.
         */
        struct MonthTitle
        {
            bool isVertical;  /**< Determine if the title is vertically rendered. */
            uint8_t textAlign;  /**< Determine the alignement of text. Left: 1, Center: 2, Right: 3. */
            uint8_t nameFormat;  /**< Determine the format of the month name accroding to
                                      MonthTitle::name_format. Default as 1.
                                  */
            QLocale locale;  /**< Locale/language to render the month name. */
            QPoint pos;  /**< Position of the title. */
            QFont font;  /**< Font use to render the title. */
            QColor textColour;  /**< Colour use to render the title. */
        };

        /**
         * Determine if two set of properties are equal.
         */
        inline bool operator==(const MonthTitle& lhs, const MonthTitle& rhs)
        {
            return (lhs.isVertical == rhs.isVertical) &&
                (lhs.textAlign == rhs.textAlign) &&
                (lhs.nameFormat == rhs.nameFormat) &&
                (lhs.locale == rhs.locale) &&
                (lhs.pos == rhs.pos) &&
                (lhs.font == rhs.font) &&
                (lhs.textColour == rhs.textColour);
        }

        /** Determine if two set of properties are not equal. */
        inline bool operator!=(const MonthTitle& lhs, const MonthTitle& rhs)
        {
            return !operator==(lhs, rhs);
        }
    }
    /**
     * @brief Represented as name of the month in various language in native locale.
     */
    class MonthTitle : public Element
    {
    public:
        /**
         * Format of name that will be rendered by locale. See Qt Documentation of QDate::toString for more
         * info.
         */
        static constexpr std::array<std::string_view, 2> name_format{ {
            "MMM",
            "MMMM"
        } };
    public:
        /**
         * Create new object with default properties.
         */
        MonthTitle();

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
         * Draw graphic for outline.
         */
        void drawOutline();
        /**
         * @internal
         * General title rendering function.
         * @param painter Painter to draw on, must not be nullptr.
         * @param date Render month in the selected date.
         */
        void drawTitle(QPainter* painter, const QDate& date);

    private:
        /**
         * @internal
         * Parent that contain the element, must not be nullptr.
         */
        CustomListWidgetItem* parent{ nullptr };
        /**
         * @internal
         * Properties of the Calendar Object.
         */
        object_properties::MonthTitle properties;
        /**
         * @internal
         * Graphic that use to render the outline of the Calendar Object.
         */
        QPixmap graphic;
    };
}