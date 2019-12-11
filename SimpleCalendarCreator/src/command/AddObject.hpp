/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <qlistwidget.h>

#include "command/Command.hpp"
#include "element/CustomListWidgetItem.hpp"

namespace command
{
	/**
	 * @brief Command that place new calendar element which decided by user to the calendar design.
	 */
	class AddObject : public Command
	{
	public:
		/**
		 * @brief Construct new command with a parent.
		 * @param list List that as the target to add new calendar element. Can't be nullptr.
		 */
		explicit AddObject(QListWidget* list);
		~AddObject() noexcept;

		bool execute() override;
		void unexecute() override;

	private:
		/**
		 * @internal
		 * @brief Targeted list to emplace calendar element, can't be nullptr.
		 */
		QListWidget* list{ nullptr };
		/**
		 * @internal
		 * @brief Tempory item that hold the id of element added to @p list.
		 */
		CustomListWidgetItem* item{ nullptr };
	};
}