/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <QColor>

#include "element/Element.hpp"

namespace element
{
    namespace object_properties
    {
        /**
         * @brief Properties of element::Ellipse
         */
        struct Ellipse
        {
            int radiusX;  /**< Horizontal radius of Ellipse. */
            int radiusY;  /**< Vertical radius of Ellipse. */
            int width;  /**< Width of border. */
            QPoint originPos;  /**< Position of origin of the Ellipse. */
            QColor foregroundColour;  /**< Border colour. */
            QColor backgroundColour;  /**< Fill colour. */
        };

        /**
         * Determine if two properties are equal.
         */
        inline bool operator==(const Ellipse& lhs, const Ellipse& rhs)
        {
            return (lhs.radiusX == rhs.radiusX) &&
                (lhs.radiusY == rhs.radiusY) &&
                (lhs.width == rhs.width) &&
                (lhs.originPos == rhs.originPos) &&
                (lhs.foregroundColour == rhs.foregroundColour) &&
                (lhs.backgroundColour == rhs.backgroundColour);
        }

        /**
         * Determine if two properties are not equal.
         */
        inline bool operator!=(const Ellipse& lhs, const Ellipse& rhs)
        {
            return !operator==(lhs, rhs);
        }
    }

    /**
     * @brief Represented as ellipse or circle in Calendar Object.
     */
    class Ellipse : public Element
    {
    public:
        /**
         * Construct new Ellipse object.
         */
        Ellipse();

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
         * Render outline for ellipse.
         */
        void drawEllipse();

    private:
        /**
         * @internal
         * Parent of the calendar object. Must not be nullptr.
         */
        CustomListWidgetItem* parent{ nullptr };
        /**
         * @internal
         * Properties of Ellipse.
         */
        object_properties::Ellipse properties;
        /**
         * @internal
         * Rendered graphic for outline.
         */
        QPixmap graphic;
    };
}