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
            int lineWidth;
            QPoint posLineStart;
            QPoint posLineEnd;
            QColor lineColour;
        };
    }
    /**
     * @brief Represented as a line on the calendar design.
     */
    class Line : public element::Element
    {
    public:
        /**
         * @brief Construct Line with default properties.
         */
        Line();
        void setParent(CustomListWidgetItem* parent) override;
        void setSize(const QSize& value) override;
        const QPixmap& getRenderedGraphics() override;
        QPixmap render(const Month& month) override;
        void edit() override;
        void serialize(pugi::xml_node* node) override;
        void deserialize(const pugi::xml_node& node) override;
    private:
        /**
         * @internal
         * @brief Render the line to the "graphic" property.
         */
        void drawLine();
    private:
        /**
         * @internal
         * @brief Parent object that holding the element.
         */
        CustomListWidgetItem* parent;
        /**
         * @internal
         * @brief Common properties of line object.
         */
        element::object_properties::Line properties;
        /**
         * @internal
         * @brief Pre-rendered graphics that use as outline of the design.
         */
        QPixmap graphic;
    };
}