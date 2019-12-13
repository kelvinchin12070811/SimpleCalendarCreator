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
    /**
     * @brief Represented as a line on the calendar design.
     */
    class Line : public element::Element
    {
    public:
        void setParent(CustomListWidgetItem* parent) override;
        void setSize(const QSize& value) override;
        const QPixmap& getRenderedGraphics() override;
        void edit() override;
    private:
        /**
         * @internal
         * @brief Render the line to the "graphic" property.
         */
        void drawLine();
        /**
         * @internal
         * @brief Slot when "Ok" button is clicked on the edit dialog.
         */
        void onAccepted(QDialog* dialog);
    private:
        /**
         * @internal
         * @brief Parent object that holding the element.
         */
        CustomListWidgetItem* parent;
        /**
         * @internal
         * @brief Thickness of the line.
         */
        int lineWidth{ 1 };
        /**
         * @internal
         * @brief Position of the line's nodes
         *  - Index 0 as starting point
         *  - Index 1 as ending point
         */
        std::array<QPoint, 2> lineNodes;
        /**
         * @internal
         * brief Colour of the line, black is default.
         */
        QColor lineColour{ Qt::GlobalColor::black };
        /**
         * @internal
         * @brief Pre-rendered graphics that use as outline of the design.
         */
        QPixmap graphic;
    };
}