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

#include <boost/assert.hpp>

#include <qcolordialog.h>
#include <qdialog.h>
#include <qfile.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qspinbox.h>

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
        auto dialogWindowTitle = dialog->windowTitle();
        dialog->setWindowTitle(dialogWindowTitle.arg(parent->text()));
        dialog->exec();
    }

    void Line::serialize(pugi::xml_node* node)
    {
        BOOST_ASSERT_MSG(node != nullptr, "node can't be nullptr");
        node->append_attribute("type").set_value(element::Element::getTypeName<Line>().c_str());
        auto ndColour = node->append_child("colour");
        ndColour.text().set(properties.lineColour.name().toUtf8().data());

        auto ndWidth = node->append_child("width");
        ndWidth.text().set(properties.lineWidth);

        auto ndPoints = node->append_child("points");
        {
            auto ndStart = ndPoints.append_child("start");
            ndStart.append_attribute("x").set_value(properties.posLineStart.x());
            ndStart.append_attribute("y").set_value(properties.posLineStart.y());

            auto ndEnd = ndPoints.append_child("end");
            ndEnd.append_attribute("x").set_value(properties.posLineEnd.x());
            ndEnd.append_attribute("y").set_value(properties.posLineEnd.y());
        }
    }

    void Line::deserialize(const pugi::xml_node& node)
    {
        properties.lineColour = node.child("colour").text().as_string();
        properties.lineWidth = node.child("width").text().as_int();
            
        auto points = node.child("points");
        auto pos1 = points.child("start");
        auto pos2 = points.child("end");

        properties.posLineStart.setX(pos1.attribute("x").as_int());
        properties.posLineStart.setY(pos1.attribute("y").as_int());
        properties.posLineEnd.setX(pos2.attribute("x").as_int());
        properties.posLineEnd.setY(pos2.attribute("y").as_int());
        drawLine();
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