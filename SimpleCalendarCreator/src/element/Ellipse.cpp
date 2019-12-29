/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/Ellipse.hpp"

#include <boost/assert.hpp>

#include "element/CustomListWidgetItem.hpp"
#include "window/object_editor/EditEllipse.hpp"

namespace element
{
    Ellipse::Ellipse()
    {
        properties = {
            0,
            0,
            1,
            QPoint{},
            Qt::GlobalColor::black,
            Qt::GlobalColor::transparent
        };
    }
    
    void Ellipse::setParent(CustomListWidgetItem* parent)
    {
        BOOST_ASSERT_MSG(parent != nullptr, "parent must not be nullptr");
        this->parent = parent;
    }
    
    void Ellipse::setSize(const QSize& size)
    {
        if (graphic.isNull())
            graphic = QPixmap{ size };
        else
            graphic.scaled(size, Qt::AspectRatioMode::KeepAspectRatio);
    }
    
    const QPixmap& Ellipse::getRenderedGraphics()
    {
        return graphic;
    }
    
    QPixmap Ellipse::render(const QDate& date)
    {
        QPixmap rendered{ graphic.size() };
        rendered.fill(Qt::GlobalColor::transparent);

        QPainter painter{ &rendered };
        QPen pen{ { properties.foregroundColour }, static_cast<qreal>(properties.width) };
        painter.setPen(pen);
        painter.setRenderHint(QPainter::RenderHint::Antialiasing);

        QPainterPath path;
        path.addEllipse(properties.originPos, static_cast<qreal>(properties.radiusX),
            static_cast<qreal>(properties.radiusY));
        painter.fillPath(path, { properties.backgroundColour });
        painter.drawPath(path);

        return rendered;
    }
    
    void Ellipse::edit()
    {
        auto dialog = std::make_unique<EditEllipse>(&properties);
        dialog->forwardConnect(std::bind(&Ellipse::drawEllipse, this));
        
        QString title{ dialog->windowTitle() };
        dialog->setWindowTitle(title.arg(parent->text()));
        dialog->exec();
    }
    
    void Ellipse::serialize(pugi::xml_node* node)
    {
        BOOST_ASSERT_MSG(node != nullptr, "node must not be nullptr");
        node->append_attribute("type").set_value(element::Element::getTypeName<element::Ellipse>().c_str());
        
        node->append_child("border_width").text().set(properties.width);

        auto nodOrigin = node->append_child("origin");
        nodOrigin.append_attribute("x").set_value(properties.originPos.x());
        nodOrigin.append_attribute("y").set_value(properties.originPos.y());

        auto nodRad = node->append_child("radius");
        nodRad.append_attribute("x").set_value(properties.radiusX);
        nodRad.append_attribute("y").set_value(properties.radiusY);

        auto nodColour = node->append_child("colour");
        nodColour.append_child("foreground").text().set(properties.foregroundColour
            .name(QColor::NameFormat::HexArgb).toUtf8().data());
        nodColour.append_child("background").text().set(properties.backgroundColour
            .name(QColor::NameFormat::HexArgb).toUtf8().data());
    }
    
    void Ellipse::deserialize(const pugi::xml_node& node)
    {
        properties.width = node.child("border_width").text().as_int();
        
        auto nodOrigin = node.child("origin");
        properties.originPos = QPoint{
            nodOrigin.attribute("x").as_int(),
            nodOrigin.attribute("y").as_int()
        };

        auto nodRad = node.child("radius");
        properties.radiusX = nodRad.attribute("x").as_int();
        properties.radiusY = nodRad.attribute("y").as_int();

        auto nodColour = node.child("colour");
        properties.foregroundColour = QColor{ nodColour.child("foreground").text().as_string() };
        properties.backgroundColour = QColor{ nodColour.child("background").text().as_string() };
        drawEllipse();
    }

    void Ellipse::drawEllipse()
    {
        graphic.fill(Qt::GlobalColor::transparent);

        QPainter painter{ &graphic };
        QPen pen{ { properties.foregroundColour }, static_cast<qreal>(properties.width) };

        QPainterPath path;
        path.addEllipse(properties.originPos, static_cast<qreal>(properties.radiusX),
            static_cast<qreal>(properties.radiusY));

        painter.setPen(pen);
        painter.fillPath(path, { properties.backgroundColour });
        painter.drawPath(path);
        parent->renderOutline();
    }
}