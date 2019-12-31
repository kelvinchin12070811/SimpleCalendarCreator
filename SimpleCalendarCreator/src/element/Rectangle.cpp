/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/Rectangle.hpp"

#include <boost/assert.hpp>

#include <QPainter>

#include "element/CustomListWidgetItem.hpp"
#include "window/object_editor/EditRectangle.hpp"

namespace element
{
    Rectangle::Rectangle()
    {
        properties.backgroundColour = QColor{ 0, 0, 0, 0 };
        properties.foregroundColour = Qt::GlobalColor::black;
        properties.rect = QRect{};
        properties.width = 1;
    }
    
    void Rectangle::setParent(CustomListWidgetItem* parent)
    {
        this->parent = parent;
    }
    
    void Rectangle::setSize(const QSize& size)
    {
        if (graphic.size().isNull())
            graphic = QPixmap{ size };
        else
            graphic.scaled(size);
    }
    
    const QPixmap& Rectangle::getRenderedGraphics()
    {
        return graphic;
    }
    
    QPixmap Rectangle::render(const QDate& date)
    {
        QPixmap rendered{ graphic.size() };
        rendered.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &rendered };
        QPen pen{ properties.foregroundColour, static_cast<qreal>(properties.width) };
        painter.setRenderHint(QPainter::RenderHint::Antialiasing);
        painter.setPen(pen);

        QPainterPath path;
        path.addRect(properties.rect);
        painter.fillPath(path, { properties.backgroundColour });
        painter.drawPath(path);

        return rendered;
    }
    
    void Rectangle::edit()
    {
        auto dialog = std::make_unique<EditRectangle>(&properties);
        auto dialogTitle = dialog->windowTitle();
        dialog->setWindowTitle(dialogTitle.arg(parent->text()));
        dialog->forwardConnect(std::bind(&Rectangle::drawRect, this));
        dialog->exec();
    }
    
    void Rectangle::serialize(pugi::xml_node* node)
    {
        BOOST_ASSERT_MSG(node != nullptr, "node must not be nullptr");
        node->append_attribute("type").set_value(element::Element::getTypeName<Rectangle>().c_str());
        node->append_child("border_width").text().set(properties.width);
        
        auto ndColour = node->append_child("colour");
        ndColour.append_child("background").text()
            .set(properties.backgroundColour.name(QColor::NameFormat::HexArgb).toUtf8().data());
        ndColour.append_child("foreground").text()
            .set(properties.foregroundColour.name(QColor::NameFormat::HexArgb).toUtf8().data());

        auto ndRect = node->append_child("rect");
        ndRect.append_attribute("x").set_value(properties.rect.x());
        ndRect.append_attribute("y").set_value(properties.rect.x());
        ndRect.append_attribute("w").set_value(properties.rect.width());
        ndRect.append_attribute("h").set_value(properties.rect.height());
    }
    
    void Rectangle::deserialize(const pugi::xml_node& node)
    {
        properties.width = node.child("border_width").text().as_int();
        
        auto ndColour = node.child("colour");
        properties.backgroundColour = QColor{ ndColour.child("background").text().as_string() };
        properties.foregroundColour = QColor{ ndColour.child("foreground").text().as_string() };

        auto ndRect = node.child("rect");
        properties.rect = QRect{
            ndRect.attribute("x").as_int(),
            ndRect.attribute("y").as_int(),
            ndRect.attribute("w").as_int(),
            ndRect.attribute("h").as_int()
        };
        drawRect();
    }
    
    void Rectangle::drawRect()
    {
        graphic.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &graphic };
        QPen pen{ properties.foregroundColour, static_cast<qreal>(properties.width) };
        painter.setPen(pen);
        
        QPainterPath path;
        path.addRect(properties.rect);
        painter.fillPath(path, { properties.backgroundColour });
        painter.drawPath(path);
        parent->renderOutline();
    }
}