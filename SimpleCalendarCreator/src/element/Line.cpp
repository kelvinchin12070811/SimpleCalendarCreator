/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include <numeric>
#include <qdialog.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <quiloader.h>
#include "CustomListWidgetItem.hpp"
#include "Line.hpp"

namespace element
{
	void Line::setParent(CustomListWidgetItem* parent)
	{
		this->parent = parent;
	}

	const QPixmap& Line::getRenderedGraphics()
	{
		return graphic;
	}
	
	void Line::edit()
	{
		QFile ui{ ":/resource/ui/edit_line.ui" };
		ui.open(QIODevice::ReadOnly);
		assert(ui.isOpen());
		QUiLoader loader;
		std::unique_ptr<QDialog> dialog{ static_cast<QDialog*>(loader.load(&ui)) };
		dialog->setWindowTitle(dialog->windowTitle().arg(parent->text()));

		dialog->connect(dialog->findChild<QPushButton*>("btnOk"), &QPushButton::clicked,
			dialog.get(), &QDialog::close);
		dialog->connect(dialog->findChild<QPushButton*>("btnCancel"), &QPushButton::clicked,
			dialog.get(), &QDialog::close);
		dialog->exec();
	}
}