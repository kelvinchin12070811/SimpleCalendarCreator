/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <array>

#include <qcolor.h>
#include <qpoint.h>

#include "element/Element.hpp"

namespace element
{
    namespace object_properties
    {
        /**
         * @brief Common properties of the calendar object element::Line.
         */
        struct Line
        {
            int lineWidth;  /**< Width of the line. */
            QPoint posLineStart;  /**< Starting point of the line. */
            QPoint posLineEnd;  /**< Ending point of the line. */
            QColor lineColour;  /**< Colour of the line. */
        };

        /**
         * Determine if two properties are equal.
         */
        inline bool operator==(const Line& lhs, const Line& rhs)
        {
            return (lhs.lineWidth == rhs.lineWidth) &&
                (lhs.posLineStart == rhs.posLineStart) &&
                (lhs.posLineEnd == rhs.posLineEnd) &&
                (lhs.lineColour == rhs.lineColour);
        }

        /**
         * Determine if two properties are not equal.
         */
        inline bool operator!=(const Line& lhs, const Line& rhs)
        {
            return !operator==(lhs, rhs);
        }
    }
    /**
     * @brief Represented as a line on the calendar design.
     */
    class Line : public element::Element
    {
    public:
        /**
         * Construct Line with default properties.
         */
        Line();
        void setParent(CustomListWidgetItem* parent) override;
        void setSize(const QSize& value) override;
        const QPixmap& getRenderedGraphics() override;
        QPixmap render(const QDate& date) override;
        void edit(QWidget* parent = nullptr) override;
        void serialize(pugi::xml_node* node) override;
        void deserialize(const pugi::xml_node& node) override;
    private:
        /**
         * @internal
         * Render the line to the "graphic" property.
         */
        void drawLine();
    private:
        /**
         * @internal
         * Parent object that holding the element.
         */
        CustomListWidgetItem* parent;
        /**
         * @internal
         * Common properties of line object.
         */
        element::object_properties::Line properties;
        /**
         * @internal
         * Pre-rendered graphics that use as outline of the design.
         */
        QPixmap graphic;
    };
}