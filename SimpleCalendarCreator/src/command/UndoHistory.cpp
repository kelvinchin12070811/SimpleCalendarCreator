/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "command/UndoHistory.hpp"

UndoHistory* UndoHistory::getInstance()
{
    static UndoHistory* instance{ new UndoHistory };
    return instance;
}

void UndoHistory::push(std::unique_ptr<command::Command> command) noexcept
{
    tracer.push(std::move(command));
    if (!tracer.top()->execute())
    {
        tracer.pop();
        return;
    }
    unsave = true;
}

void UndoHistory::pop() noexcept
{
    tracer.top()->unexecute();
    tracer.pop();

    //Identify if the last operation is the first operation. Mark as no changes if true.
    if (tracer.empty()) unsave = false;
    else unsave = true;
}

bool UndoHistory::hasUnsave() noexcept
{
    return unsave;
}

void UndoHistory::changesSaved() noexcept
{
    unsave = false;
}

void UndoHistory::clearHistory() noexcept
{
    tracer.swap(decltype(tracer){});
}
