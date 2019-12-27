/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/Text.hpp"

#include <boost/assert.hpp>

#include "element/CustomListWidgetItem.hpp"
#include "window/object_editor/EditText.hpp"

namespace element
{
    Text::Text()
    {
        properties = {
            Qt::GlobalColor::black,
            QFont{},
            QPoint{ 0, 0 },
            QString{}
        };
    }
    
    void Text::setParent(CustomListWidgetItem* parent)
    {
        this->parent = parent;
    }
    
    void Text::setSize(const QSize& size)
    {
        if (graphic.isNull())
            graphic = QPixmap{ size };
        else
            graphic.scaled(size, Qt::AspectRatioMode::KeepAspectRatio);
    }
    
    const QPixmap& Text::getRenderedGraphics()
    {
        return graphic;
    }
    
    QPixmap Text::render(const Month& month)
    {
        QPixmap rendered{ graphic.size() };
        rendered.fill(Qt::GlobalColor::transparent);

        QPainter painter{ &rendered };
        QPen pen{ properties.textColour, static_cast<qreal>(properties.font.weight()) };
        QFontMetrics metrics{ properties.font };
        QRect textRect{ properties.pos, QSize{ metrics.width(properties.text), metrics.height() } };

        painter.setPen(pen);
        painter.setRenderHint(QPainter::RenderHint::TextAntialiasing);
        painter.setFont(properties.font);
        painter.drawText(textRect, Qt::AlignCenter, properties.text);

        return rendered;
    }
    
    void Text::edit()
    {
        auto dialog = std::make_unique<EditText>(&properties);
        QString title = dialog->windowTitle().arg(parent->text());
        dialog->setWindowTitle(title);
        dialog->forwardConnect(std::bind(&Text::drawText, this));
        dialog->exec();
    }
    
    void Text::serialize(pugi::xml_node* node)
    {
        BOOST_ASSERT_MSG(node != nullptr, "node must not be nullptr");
        node->append_attribute("type").set_value(Element::getTypeName<Text>().c_str());
        node->append_child("colour").text().set(properties.textColour.name(QColor::NameFormat::HexArgb)
            .toStdString().c_str());
        node->append_child("font").text().set(properties.font.toString().toStdString().c_str());

        auto nodPos = node->append_child("position");
        nodPos.append_attribute("x").set_value(properties.pos.x());
        nodPos.append_attribute("y").set_value(properties.pos.y());

        node->append_child("text").text().set(properties.text.toStdString().c_str());
    }
    
    void Text::deserialize(const pugi::xml_node& node)
    {
        properties.textColour = QColor{ node.child("colour").text().as_string() };
        properties.font.fromString(node.child("font").text().as_string());

        auto nodPos = node.child("position");
        properties.pos = { nodPos.attribute("x").as_int(), nodPos.attribute("y").as_int() };

        properties.text = node.child("text").text().as_string();

        drawText();
    }
    
    void Text::drawText()
    {
        graphic.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &graphic };
        QFontMetrics metrics{ properties.font };
        QRect fontRect{
            properties.pos,
            QSize{
                metrics.width(properties.text),
                metrics.height()
            }
        };
        
        QPainterPath path;
        path.addRect(fontRect);
        painter.fillPath(path, { Text::text_preview_rect_colour });
        painter.setFont(properties.font);
        painter.drawText(fontRect, parent->text());
        parent->renderOutline();
    }
}