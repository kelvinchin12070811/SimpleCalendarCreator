/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <qpixmap.h>

#include "element/Month.hpp"

class CustomListWidgetItem;

namespace element
{
    /**
     * @brief Interface of all calendar elements.
     *
     * The Elements is created only via the "ObjectCreator" window by user.
     *
     * @warning It should be use only with dynamically allocated memory.
     */
    class Element
    {
    public:
        /**
         * @brief Set parent of the element.
         */
        virtual void setParent(CustomListWidgetItem* parent) = 0;
        /**
         * @brief Resize the element.
         */
        virtual void setSize(const QSize& value) = 0;
        /**
         * @brief Get the rendered graphics for outline purpose.
         */
        virtual const QPixmap& getRenderedGraphics() = 0;
        /**
         * @brief Render the selected month.
         */
        virtual QPixmap render(const Month& month) = 0;
        /**
         * @brief Allow user to modifide the properties of the element.
         */
        virtual void edit() = 0;
        virtual ~Element() noexcept = 0;
    };
    inline Element::~Element() = default;
}