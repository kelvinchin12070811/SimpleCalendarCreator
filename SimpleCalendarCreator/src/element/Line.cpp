/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/CustomListWidgetItem.hpp"

#include <algorithm>
#include <functional>
#include <map>
#include <numeric>

#include <qcolordialog.h>
#include <qdialog.h>
#include <qfile.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <quiloader.h>

#include "element/Line.hpp"
#include "window/object_editor/EditLine.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif // _DEBUG

namespace element
{
    Line::Line()
    {
        properties.lineColour = Qt::GlobalColor::black;
        properties.lineWidth = 1;
        properties.posLineStart = QPoint{};
        properties.posLineEnd = QPoint{};
    }

    void Line::setParent(CustomListWidgetItem* parent)
    {
        this->parent = parent;
        graphic = QPixmap{};
    }

    void Line::setSize(const QSize& value)
    {
        if (graphic.size().isNull())
            graphic = QPixmap{ value };
        else
            graphic.scaled(value, Qt::AspectRatioMode::KeepAspectRatio);
    }

    const QPixmap& Line::getRenderedGraphics()
    {
        return graphic;
    }

    QPixmap Line::render(const Month& month)
    {
        QPixmap rendered{ graphic.size() };
        rendered.fill(Qt::GlobalColor::transparent);

        QPainter painter{ &rendered };
        QPen pen{ painter.pen() };
        pen.setWidth(properties.lineWidth);
        pen.setColor(properties.lineColour);
        painter.setPen(pen);

        painter.drawLine(properties.posLineStart, properties.posLineEnd);

        return rendered;
    }

    void Line::edit()
    {
        auto dialog = std::make_unique<EditLine>(&properties);
        dialog->forwardConnect(std::bind(&Line::drawLine, this));
        dialog->exec();
    }

    void Line::drawLine()
    {
        graphic.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &graphic };
        QPen pen = painter.pen();
        pen.setColor(properties.lineColour);
        pen.setWidth(properties.lineWidth);
        painter.setPen(pen);
        painter.drawLine(properties.posLineStart, properties.posLineEnd);
        parent->renderOutline();
    }
}