/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <boost/assert.hpp>
#include <boost/signals2.hpp>

#include "command/Command.hpp"

namespace command
{
    /**
     * @brief Command as general properties changer for Calendar Objects.
     * Emit signal when the properties of a calendar object has been modified.
     * @note This class does not use Qt's signal-slot pattern instade of using boost::signals2 due to the
     * limitations of template class is not able to extend QObject.
     */
    template <typename CalendarObjectProperties>
    class ChangeObjectProperties : public Command
    {
    public:
        /**
         * Construct new ChangeObjectProperties object.
         * @param curProperties Observer pointer to current calendar object's properties that will be
         * changed, must not be nullptr.
         * @param newProperties New properties value to change.
         */
        ChangeObjectProperties(CalendarObjectProperties* curProperties,
            const CalendarObjectProperties& newProperties):
            curProperties(curProperties), prevProperties(*curProperties), newProperties(newProperties)
        {
            BOOST_ASSERT_MSG(curProperties != nullptr, "curProperties must not be nullptr");
        }

        bool execute() override
        {
            *curProperties = newProperties;
            propertiesChanged();
            return true;
        }

        void unexecute() override
        {
            *curProperties = prevProperties;
            propertiesChanged();
        }

    public:  //Signals
        /**
         * @name Signals
         * @{
         */
        /**
         * Fired when the targeted calendar object's properties changed.
         */
        boost::signals2::signal<void()> propertiesChanged;
        /** @} */
    private:
        /**
         * @internal
         * Previous properties of calendar object.
         */
        CalendarObjectProperties prevProperties;
        /**
         * @internal
         * New properties of calendar object which will be changed.
         */
        CalendarObjectProperties newProperties;
        /**
         * @internal
         * Observer to Calendar Object's properties that will be changed.
         */
        CalendarObjectProperties* curProperties;
    };
}