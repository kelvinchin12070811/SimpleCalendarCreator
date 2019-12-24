/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <qlistwidget.h>

#include "command/Command.hpp"
#include "element/CustomListWidgetItem.hpp"
#include "window/SimpleCalendarCreator.hpp"

namespace command
{
    /**
     * @brief Command that place new calendar element which decided by user to the calendar design.
     */
    class AddObject : public Command
    {
    public:
        /**
         * Construct new command with a parent.
         * @param list List that as the target to add new calendar element. Can't be nullptr.
         * @param mainWindow Reference to the mainWindow object for rendering outline, can't be nullptr.
         */
        explicit AddObject(SimpleCalendarCreator* mainWindow, QListWidget* list);
        ~AddObject() noexcept;

        bool execute() override;
        void unexecute() override;

    private:
        /**
         * @internal
         * Targeted list to emplace calendar element, can't be nullptr.
         */
        QListWidget* list{ nullptr };
        /**
         * @internal
         * Tempory item that hold the id of element added to @p list.
         */
        CustomListWidgetItem* item{ nullptr };
    };
}