/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/TemplatedText.hpp"

#include <algorithm>

#include <boost/assert.hpp>

#include <QDate>
#include <QFontMetrics>

#include "element/CustomListWidgetItem.hpp"
#include "element/Text.hpp"
#include "window/object_editor/EditTemplatedText.hpp"

namespace element
{
    TemplatedText::TemplatedText()
    {
        properties = {
            false,
            1,
            Qt::GlobalColor::black,
            QFont{},
            QPoint{},
            QStringList{}
        };
    }
    
    void TemplatedText::setParent(CustomListWidgetItem* parent)
    {
        BOOST_ASSERT_MSG(parent != nullptr, "parent must not be nullptr");
        this->parent = parent;
    }
    
    void TemplatedText::setSize(const QSize& size)
    {
        if (graphic.isNull())
            graphic = QPixmap{ size };
        else
            graphic.scaled(size);
    }
    
    const QPixmap& TemplatedText::getRenderedGraphics()
    {
        return graphic;
    }
    
    QPixmap TemplatedText::render(const QDate& date)
    {
        QPixmap rendered{ graphic.size() };
        rendered.fill(Qt::GlobalColor::transparent);

        QPainter painter{ &rendered };
        painter.setRenderHint(QPainter::RenderHint::TextAntialiasing);
        drawText(&painter, date);
        return rendered;
    }
    
    void TemplatedText::edit(QWidget* parent)
    {
        auto dialog = std::make_unique<EditTemplatedText>(&properties, parent);
        QString title{ dialog->windowTitle().arg(this->parent->text()) };

        dialog->setWindowTitle(title);
        dialog->forwardConnect(std::bind(&TemplatedText::drawOutline, this));
        dialog->exec();
    }
    
    void TemplatedText::serialize(pugi::xml_node* node)
    {
        BOOST_ASSERT_MSG(node != nullptr, "node must not be nullptr");
        node->append_attribute("type").set_value(Element::getTypeName<TemplatedText>().c_str());
        node->append_child("colour").text().set(properties.textColour.name(QColor::HexArgb)
            .toStdString().c_str());
        node->append_child("font").text().set(properties.font.toString().toStdString().c_str());
        
        auto nodPos = node->append_child("position");
        nodPos.append_attribute("x").set_value(properties.pos.x());
        nodPos.append_attribute("y").set_value(properties.pos.y());

        auto nodText = node->append_child("text");
        nodText.append_attribute("vertical").set_value(properties.isVertical);
        nodText.append_attribute("text-align").set_value(properties.textAlign);

        int counter{ 0 };
        for (auto& itr : properties.texts)
        {
            if (counter >= 12) break;
            auto nodTextChild = nodText.append_child("item");
            nodTextChild.text().set(itr.toStdString().c_str());
            counter++;
        }
    }
    
    void TemplatedText::deserialize(const pugi::xml_node& node)
    {
        properties.textColour = QColor{ node.child("colour").text().as_string() };
        properties.font.fromString(node.child("font").text().as_string());

        auto nodPos = node.child("position");
        properties.pos = QPoint{
            nodPos.attribute("x").as_int(),
            nodPos.attribute("y").as_int()
        };

        auto nodText = node.child("text");
        properties.isVertical = nodText.attribute("vertical").as_bool();
        properties.textAlign = static_cast<uint8_t>(nodText.attribute("text-align").as_uint(1));
        
        int idx{ 0 };
        for (auto& itr : nodText.children())
        {
            using namespace std::string_literals;
            if (idx >= 12) break;
            if (itr.name() != "item"s) continue;

            properties.texts.push_back(itr.text().as_string());
            idx++;
        }
        drawOutline();
    }

    void TemplatedText::drawOutline()
    {
        graphic.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &graphic };
        QDate date{ QDate::currentDate() };
        date.setDate(date.year(), 1, 1);
        drawText(&painter, date);
        parent->renderOutline();
    }

    void TemplatedText::drawText(QPainter* painter, const QDate& date)
    {
        BOOST_ASSERT_MSG(painter != nullptr, "painter must not be nullptr");
        int idx{ std::clamp(date.month(), 1, 12) - 1 };
        if (idx >= properties.texts.size())
            idx = idx % properties.texts.size();

        QPen pen{ properties.textColour };
        QFontMetrics metrics{ properties.font };
        QString textToRender{ properties.texts.at(idx) };

        painter->setPen(pen);
        painter->setFont(properties.font);

        QRect fontRect{ properties.pos, QSize{} };
        int shift{ 0 };
        if (!properties.isVertical)
        {
            fontRect.setWidth(metrics.width(textToRender));
            fontRect.setHeight(metrics.height());
            switch (properties.textAlign)
            {
            case 2:
                shift = fontRect.width() / 2;
                break;
            case 3:
                shift = fontRect.width();
                break;
            }
            painter->drawText(fontRect.x() - shift, fontRect.y(), fontRect.width(), fontRect.height(),
                0, textToRender);
        }
        else
        {
            for (const auto& itr : textToRender)
            {
                fontRect.setWidth(metrics.width(itr));
                fontRect.setHeight(metrics.height());
                switch (properties.textAlign)
                {
                case 2:
                    shift = fontRect.width() / 2;
                    break;
                case 3:
                    shift = fontRect.width();
                    break;
                }
                painter->drawText(fontRect.x() - shift, fontRect.y(), fontRect.width(), fontRect.height(),
                    0, itr);
                fontRect.setY(fontRect.y() + fontRect.height() - metrics.descent());
            }
        }
    }
}