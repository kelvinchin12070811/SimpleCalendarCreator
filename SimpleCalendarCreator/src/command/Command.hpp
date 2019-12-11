/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once

namespace command
{
    /**
     * @brief Interface of all command object.
     *
     * A command usually created at first to execute the command and then is pushed into an undo stack for
     * history tracking purpose. If the execution failed (return false), the command is not pushed to the
     * stack since the command is "unfinised" or "failed". When undo a command, the top command's unexecute()
     * on the stack is called and then that command is popped from the stack.
     *
     * @warning All command object should be dynamically created.
     */
    class Command
    {
    public:
        /**
         * @brief Execute the command.
         * @retval true if success.
         */
        virtual bool execute() = 0;
        /**
         * @brief Undo this command.
         */
        virtual void unexecute() = 0;
        virtual ~Command() noexcept = 0;
    };
    inline Command::~Command() noexcept = default;
}