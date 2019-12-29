/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
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
            QLocale locale;  /**< Locale/language to render the month name. */
            QPoint pos;  /**< Position of the title. */
            QFont font;  /**< Font use to render the title. */
            QColor textColour;  /**< Colour use to render the title. */
        };
    }
    /**
     * @brief Represented as name of the month in various language in native locale.
     */
    class MonthTitle : public Element
    {
    public:
        /**
         * Create new object with default properties.
         */
        MonthTitle();

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
         * Draw graphic for outline.
         */
        void drawOutline();
        /**
         * @internal
         * General title rendering function.
         * @param painter Painter to draw on, must not be nullptr.
         * @parma date Render month in the selected date.
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