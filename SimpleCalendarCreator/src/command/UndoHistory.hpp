/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>
#include <stack>

#include "command/Command.hpp"
/**
 * @brief Singletone object that hold the history of all user's operation and provide undo functionality.
 */
class UndoHistory
{
public:
    UndoHistory(const UndoHistory&) = delete;
    UndoHistory(UndoHistory&&) = delete;
    UndoHistory& operator=(const UndoHistory&) = delete;
    UndoHistory& operator=(UndoHistory&&) = delete;

    /**
     * Get the singletone instance of Undo History
     */
    static UndoHistory* getInstance();

    /**
     * Push command to undo stack and execute the operation.
     */
    void push(std::unique_ptr<command::Command> command) noexcept;
    /**
     * Pop command from undo stack and revert the operation.
     */
    void pop() noexcept;

    /**
     * Determine if user have unsave changes, traced by command pushed to undo stack.
     */
    bool hasUnsave() noexcept;
    /**
     * Update the unsave status to saved.
     */
    void changesSaved() noexcept;
    /**
     * Clear all undo history.
     */
    void clearHistory() noexcept;
protected:
    ~UndoHistory() noexcept = default;
private:
    UndoHistory() = default;

private:  //Attributes
    /**
     * @internal
     * Determine if user have unchange.
     */
    bool unsave{ false };
    /**
     * @internal
     * Stack to trace user's operations.
     */
    std::stack<std::unique_ptr<command::Command>> tracer;
};