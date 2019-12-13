/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>

#include <qlistwidget.h>
#include <qgraphicsview.h>

#include "command/Command.hpp"
#include "element/CustomListWidgetItem.hpp"

namespace command
{
    /**
     * @brief Command that remove selected calendar element from the design.
     */
    class RemoveObject : public Command
    {
    public:
        /**
         * @brief Construct new remove object command.
         * @param list Targeted list to remove the user selected object, can't be nullptr.
         * @param view Reference to outline window to remove outline. Can't be nullptr.
         */
        explicit RemoveObject(QListWidget* list, QGraphicsView* view);
        ~RemoveObject() noexcept = default;

        bool execute() override;
        void unexecute() override;
    private:
        /**
         * @internal
         * @brief Temporary holder of item returned from the list.
         * Use to restore the item to the list when unexecute() called.
         */
        std::unique_ptr<CustomListWidgetItem> item{ nullptr };
        /**
         * @internal
         * @brief Targeted list to perform removal, can't be nullptr.
         */
        QListWidget* list{ nullptr };
        /**
         * @internal
         * @brief Previous index of the removed item, use to reinsert the item to the list when unexeute().
         */
        QModelIndex index;

        /**
         * @internal
         * @brief Reference to outline window to remove outline from preview. Can't be nullptr.
         */
        QGraphicsView* view{ nullptr };
    };
}