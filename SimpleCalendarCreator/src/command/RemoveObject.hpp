/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>

#include <qlistwidget.h>

#include "command/Command.hpp"

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
		 */
		explicit RemoveObject(QListWidget* list);
		~RemoveObject() noexcept = default;

		bool execute() override;
		void unexecute() override;
	private:
		/**
		 * @internal
		 * @brief Temporary holder of item returned from the list.
		 * Use to restore the item to the list when unexecute() called.
		 */
		std::unique_ptr<QListWidgetItem> item{ nullptr };
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
	};
}