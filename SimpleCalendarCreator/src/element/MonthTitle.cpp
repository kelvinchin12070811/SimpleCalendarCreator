/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/MonthTitle.hpp"

#include <boost/assert.hpp>

#include <QDate>

#include "element/CustomListWidgetItem.hpp"
#include "window/object_editor/EditMonthTitle.hpp"

namespace element
{
    MonthTitle::MonthTitle()
    {
        properties = {
            false,
            1,
            1,
            QLocale{ QLocale::Language::English, QLocale::Country::UnitedKingdom },
            QPoint{},
            QFont{},
            Qt::GlobalColor::black
        };
    }
    
    void MonthTitle::setParent(CustomListWidgetItem* parent)
    {
        BOOST_ASSERT_MSG(parent != nullptr, "parent must not be nullptr");
        this->parent = parent;
    }
    
    void MonthTitle::setSize(const QSize& size)
    {
        if (graphic.isNull())
            graphic = QPixmap{ size };
        else
            graphic.scaled(size, Qt::AspectRatioMode::KeepAspectRatio);
    }
    
    const QPixmap& MonthTitle::getRenderedGraphics()
    {
        return graphic;
    }
    
    QPixmap MonthTitle::render(const QDate& date)
    {
        QPixmap rendered{ graphic.size() };
        rendered.fill(Qt::GlobalColor::transparent);

        QPainter painter{ &rendered };
        painter.setRenderHint(QPainter::RenderHint::TextAntialiasing);
        drawTitle(&painter, date);

        return rendered;
    }
    
    void MonthTitle::edit()
    {
        auto dialog = std::make_unique<EditMonthTitle>(&properties);
        QString title{ dialog->windowTitle().arg(parent->text()) };
        dialog->setWindowTitle(title);
        dialog->forwardConnect(std::bind(&MonthTitle::drawOutline, this));
        dialog->exec();
    }
    
    void MonthTitle::serialize(pugi::xml_node* node)
    {
        BOOST_ASSERT_MSG(node != nullptr, "node must not be nullptr");
        node->append_attribute("type").set_value(Element::getTypeName<MonthTitle>().c_str());
        node->append_child("month-name-format").text().set(properties.nameFormat);
        node->append_child("locale").text().set(properties.locale.name().toStdString().c_str());
        
        auto nodPos = node->append_child("position");
        nodPos.append_attribute("x").set_value(properties.pos.x());
        nodPos.append_attribute("y").set_value(properties.pos.y());

        node->append_child("font").text().set(properties.font.toString().toStdString().c_str());
        node->append_child("colour").text().set(properties.textColour.name(QColor::NameFormat::HexArgb)
            .toStdString().c_str());
        
        auto nodText = node->append_child("text");
        nodText.append_attribute("vertical").set_value(properties.isVertical);
        nodText.append_attribute("text-align").set_value(properties.textAlign);
    }
    
    void MonthTitle::deserialize(const pugi::xml_node& node)
    {
        properties.nameFormat = static_cast<uint8_t>(node.child("month-name-format").text().as_uint());
        properties.locale = QLocale{ node.child("locale").text().as_string() };

        auto nodPos = node.child("position");
        properties.pos = QPoint{
            nodPos.attribute("x").as_int(),
            nodPos.attribute("y").as_int()
        };

        properties.font.fromString(node.child("font").text().as_string());
        properties.textColour = QColor{ node.child("colour").text().as_string() };
        
        auto nodText = node.child("text");
        properties.isVertical = nodText.attribute("vertical").as_bool();
        properties.textAlign = static_cast<uint8_t>(nodText.attribute("text-align").as_uint(1));

        drawOutline();
    }

    void MonthTitle::drawOutline()
    {
        graphic.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &graphic };
        auto date = QDate::currentDate();
        date.setDate(date.year(), 1, 1);
        drawTitle(&painter, date);
        parent->renderOutline();
    }
    
    void MonthTitle::drawTitle(QPainter* painter, const QDate& date)
    {
        QPen pen{ properties.textColour };
        painter->setPen(pen);
        painter->setFont(properties.font);

        QFontMetrics metrics{ properties.font };
        QString monthName{ properties.locale.toString(date, MonthTitle::name_format[properties.nameFormat]
            .data()) };
        QRect fontRect{ properties.pos, QSize{} };
        int shift{ 0 };
        if (!properties.isVertical)
        {
            fontRect.setWidth(metrics.width(monthName));
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
                0, monthName);
        }
        else
        {
            for (const auto& itr : monthName)
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