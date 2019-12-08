/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>
#include <qlistwidget.h>
#include "Command.hpp"

namespace command
{
	class RemoveObject : public Command
	{
	public:
		explicit RemoveObject(QListWidget& list);
		~RemoveObject() = default;

		bool execute() override;
		void unexecute() override;
	private:
		std::unique_ptr<QListWidgetItem> item{ nullptr };
		QListWidget* list{ nullptr };
		QModelIndex index;
	};
}