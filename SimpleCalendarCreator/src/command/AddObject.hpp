/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <qlistwidget.h>
#include "Command.hpp"

namespace command
{
	class AddObject : public Command
	{
	public:
		explicit AddObject(QListWidget& list);
		~AddObject();

		bool execute() override;
		void unexecute() override;

	private:
		QListWidget* list{ nullptr };
		QListWidgetItem* item{ nullptr };
	};
}