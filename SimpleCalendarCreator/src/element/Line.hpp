/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <array>
#include <qcolor.h>
#include <qpoint.h>
#include "Element.hpp"

namespace element
{
	class Line : public Element
	{
	public:
		void setParent(CustomListWidgetItem* parent) override;
		const QPixmap& getRenderedGraphics() override;
		void edit() override;
	private:
		void drawLine();
		void onAccepted(QDialog* dialog);
	private:
		CustomListWidgetItem* parent;
		int lineWidth{ 1 };
		std::array<QPoint, 2> lineNodes;
		QColor lineColour{ Qt::GlobalColor::black };
		QPixmap graphic;
	};
}