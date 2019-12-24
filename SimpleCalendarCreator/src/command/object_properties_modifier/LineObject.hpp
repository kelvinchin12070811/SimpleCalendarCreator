/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <qobject.h>

#include "command/Command.hpp"
#include "element/Line.hpp"

namespace command::object_properties_modifier
{
    /**
     * @brief Command that modify the properties of element::Line calendar object.
     */
    class LineObject : public QObject, public command::Command
    {
        Q_OBJECT
    public:
        /**
         * Construct new LineObject modifier.
         * @param properties Reference to target properties to change, can't be nullptr.
         * @param newValue New values to apply.
         */
        LineObject(element::object_properties::Line* properties,
            const element::object_properties::Line& newValue);
        ~LineObject() noexcept = default;

        bool execute() override;
        void unexecute() override;
    signals:
        /**
         * @name Signals
         * @{
         */
        /**
         * Fired when the properties changed via execute or unexecute.
         */
        void propertiesChanged();
        /** @} */
    private:
        /**
         * @internal
         * Last properties of object.
         */
        element::object_properties::Line prevProperties;
        /**
         * @internal
         * new properties value to apply.
         */
        element::object_properties::Line newProperties;
        /**
         * @internal
         * Reference to properties to change.
         */
        element::object_properties::Line* curnProperties;
    };
}