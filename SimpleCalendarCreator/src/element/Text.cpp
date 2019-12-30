/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/Text.hpp"

#include <boost/assert.hpp>

#include <QFontMetrics>
#include <QRect>

#include "element/CustomListWidgetItem.hpp"
#include "window/object_editor/EditText.hpp"

namespace element
{
    Text::Text()
    {
        properties = {
            false,
            1,
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
    
    QPixmap Text::render(const QDate& date)
    {
        QPixmap rendered{ graphic.size() };
        rendered.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &rendered };
        painter.setRenderHint(QPainter::RenderHint::TextAntialiasing);
        drawText(&painter);
        return rendered;
    }
    
    void Text::edit()
    {
        auto dialog = std::make_unique<EditText>(&properties);
        QString title = dialog->windowTitle().arg(parent->text());
        dialog->setWindowTitle(title);
        dialog->forwardConnect(std::bind(&Text::drawOutline, this));
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

        auto nodText = node->append_child("text");
        nodText.text().set(properties.text.toStdString().c_str());
        nodText.append_attribute("vertical").set_value(properties.verticalText);
        nodText.append_attribute("text-align").set_value(properties.textAlignment);
    }
    
    void Text::deserialize(const pugi::xml_node& node)
    {
        properties.textColour = QColor{ node.child("colour").text().as_string() };
        properties.font.fromString(node.child("font").text().as_string());

        auto nodPos = node.child("position");
        properties.pos = { nodPos.attribute("x").as_int(), nodPos.attribute("y").as_int() };

        auto nodText = node.child("text");
        properties.text = nodText.text().as_string();
        properties.verticalText = nodText.attribute("vertical").as_bool();
        properties.textAlignment = nodText.attribute("text-align").as_uint(1);

        drawOutline();
    }

    void Text::drawOutline()
    {
        graphic.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &graphic };
        drawText(&painter);
        parent->renderOutline();
    }
    
    void Text::drawText(QPainter* painter)
    {
        BOOST_ASSERT_MSG(painter != nullptr, "painter must not be nullptr");
        QFontMetrics metrics{ properties.font };
        QPen pen{ properties.textColour };
        painter->setPen(pen);
        painter->setFont(properties.font);
        if (!properties.verticalText)
        {
            QRect fontRect{
                properties.pos,
                QSize{
                    metrics.width(properties.text),
                    metrics.height()
                }
            };
            switch (properties.textAlignment)
            {
            case 2:
                fontRect.setX(fontRect.x() - (fontRect.width() / 2));
                break;
            case 3:
                fontRect.setX(fontRect.x() + fontRect.width());
                break;
            }
            painter->drawText(fontRect, properties.text);
        }
        else
        {
            QRect fontRect{ properties.pos, QSize{} };
            int shift{ 0 };
            for (const auto& itr : properties.text)
            {
                fontRect.setWidth(metrics.width(itr));
                fontRect.setHeight(metrics.height());
                if (properties.textAlignment == 2)
                    shift = fontRect.width() / 2;
                else if (properties.textAlignment == 3)
                    shift = fontRect.width();

                painter->drawText(fontRect.x() - shift, fontRect.y(), fontRect.width(), fontRect.height(),
                    0, itr);
                fontRect.setY(fontRect.y() + fontRect.height() - metrics.descent());
            }
        }
    }
}