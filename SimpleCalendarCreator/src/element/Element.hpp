/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <qpixmap.h>

class CustomListWidgetItem;

namespace element
{
	struct Element
	{
		virtual void setParent(CustomListWidgetItem* parent) = 0;
		virtual const QPixmap& getRenderedGraphics() = 0;
		virtual void edit() = 0;
		virtual ~Element() = default;
	};
}