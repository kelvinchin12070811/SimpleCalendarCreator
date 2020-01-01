/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/Dates.hpp"

#include <algorithm>
#include <array>

#include <QDate>

#include "element/CustomListWidgetItem.hpp"
#include "window/object_editor/EditDates.hpp"

#ifdef _DEBUG
#include <QDebug>
#endif // _DEBUG

namespace element
{
    Dates::Dates()
    {
        properties = element::object_properties::Dates{
            1,
            Qt::GlobalColor::black,
            Qt::GlobalColor::blue,
            Qt::GlobalColor::red,
            QFont{},
            QRect{},
            {}
        };
    }
    
    void Dates::setParent(CustomListWidgetItem* parent)
    {
        BOOST_ASSERT_MSG(parent != nullptr, "parent must not be nullptr");
        this->parent = parent;
    }
    
    void Dates::setSize(const QSize& size)
    {
        if (graphic.isNull())
            graphic = QPixmap{ size };
        else
            graphic.scaled(size);
    }
    
    const QPixmap& Dates::getRenderedGraphics()
    {
        return graphic;
    }
    
    QPixmap Dates::render(const QDate& date)
    {
        QPixmap rendered{ graphic.size() };
        rendered.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &rendered };
        painter.setRenderHint(QPainter::RenderHint::TextAntialiasing);
        drawLabels(&painter, date);
        return rendered;
    }

    void Dates::edit()
    {
        auto dialog = std::make_unique<EditDates>(&properties);
        QString title{ dialog->windowTitle().arg(parent->text()) };
        dialog->setWindowTitle(title);
        dialog->forwardConnect(std::bind(&Dates::drawOutline, this));
        dialog->exec();
    }
    
    void Dates::serialize(pugi::xml_node* node)
    {
        BOOST_ASSERT_MSG(node != nullptr, "node must not be nullptr");
        node->append_attribute("type").set_value(Element::getTypeName<Dates>().c_str());
        
        auto nodColour = node->append_child("colour");
        nodColour.append_child("weakday").text().set(properties.weakdayColour
            .name(QColor::NameFormat::HexArgb).toStdString().c_str());
        nodColour.append_child("weakend").text().set(properties.weakendColour
            .name(QColor::NameFormat::HexArgb).toStdString().c_str());
        nodColour.append_child("weakstart").text().set(properties.weakstartColour
            .name(QColor::NameFormat::HexArgb).toStdString().c_str());

        node->append_child("font").text().set(properties.font.toString().toStdString().c_str());
        
        auto nodRect = node->append_child("render-area");
        nodRect.append_attribute("x").set_value(properties.drawArea.x());
        nodRect.append_attribute("y").set_value(properties.drawArea.y());
        nodRect.append_attribute("w").set_value(properties.drawArea.width());
        nodRect.append_attribute("h").set_value(properties.drawArea.height());

        auto nodSpDates = node->append_child("special-days");
        nodSpDates.append_attribute("text-alignment").set_value(properties.textAlign);
        for (const auto& itr : properties.speacialDays)
        {
            using TupleItem = element::object_properties::Dates::SpeacialDaysIndex;
            auto markersGroup = nodSpDates.append_child("markers-group");
            markersGroup.append_attribute("name").set_value(std::get<TupleItem::group_name>(itr)
                .toStdString().c_str());
            markersGroup.append_attribute("marker-colour").set_value(std::get<TupleItem::group_colour>(itr)
                .toStdString().c_str());
            for (const auto& [name, date] : std::get<TupleItem::group_members>(itr))
            {
                auto dateEvent = markersGroup.append_child("event");
                dateEvent.append_child("name").text().set(name.toStdString().c_str());
                dateEvent.append_child("date").text().set(date.toStdString().c_str());
            }
        }
    }
    
    void Dates::deserialize(const pugi::xml_node& node)
    {
        auto nodColour = node.child("colour");
        properties.weakdayColour = QColor{ nodColour.child("weakday").text().as_string() };
        properties.weakendColour = QColor{ nodColour.child("weakend").text().as_string() };
        properties.weakstartColour = QColor{ nodColour.child("weakstart").text().as_string() };

        properties.font.fromString(node.child("font").text().as_string());

        auto nodRect = node.child("render-area");
        properties.drawArea = QRect{
            nodRect.attribute("x").as_int(),
            nodRect.attribute("y").as_int(),
            nodRect.attribute("w").as_int(),
            nodRect.attribute("h").as_int()
        };

        auto nodSpDates = node.child("special-days");
        properties.textAlign = static_cast<uint8_t>(nodSpDates.attribute("text-alignment").as_uint(1));
        for (const auto& itr : nodSpDates.children("markers-group"))
        {
            QString name{ itr.attribute("name").as_string() };
            QString colour{ itr.attribute("marker-colour").as_string() };
            std::vector<std::pair<QString, QString>> members;
            for (const auto& itr2 : itr.children("event"))
            {
                members.emplace_back(itr2.child("name").text().as_string(),
                    itr2.child("date").text().as_string());
            }
            members.shrink_to_fit();
            properties.speacialDays.emplace_back(std::move(name), std::move(colour), std::move(members));
        }
        properties.speacialDays.shrink_to_fit();
        drawOutline();
    }
    
    void Dates::drawOutline()
    {
        graphic.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &graphic };
        QDate date{ QDate::currentDate().year(), 1, 1 };
        painter.fillRect(properties.drawArea, QColor{ Dates::outline_bound_colour });
        drawLabels(&painter, date);
        parent->renderOutline();
    }
    
    void Dates::drawLabels(QPainter* painter, const QDate& date)
    {
#ifdef _DEBUG
        qDebug() << date.toString(Qt::DateFormat::ISODate);
#endif // _DEBUG
        int8_t weakstart{ 7 };  //Determine start of the weak, 1 as monday -> 7 as sunday.
        int8_t weakend{ weakstart - 1 };
        if (weakend <= 0)
            weakend = 7;

        QPen pen;
        QDate calendar{ date };
        int textAlignFlags = Qt::AlignmentFlag::AlignVCenter;

        switch (properties.textAlign)
        {
        case 2:
            textAlignFlags |= Qt::AlignmentFlag::AlignHCenter;
            break;
        case 3:
            textAlignFlags |= Qt::AlignmentFlag::AlignRight;
            break;
        default:
            textAlignFlags |= Qt::AlignmentFlag::AlignLeft;
            break;
        }

        painter->setFont(properties.font);

        std::array<int, 7> shifter{ { 0, 1, 2, 3, 4, 5, 6 } };
        std::rotate(shifter.rbegin(), shifter.rbegin() + (weakstart - 1), shifter.rend());
        
        int x{ properties.drawArea.x() };
        int y{ properties.drawArea.y() };
        int w{ properties.drawArea.width() / 7 };
        int h{ properties.drawArea.height() / 5 };
        while (calendar.month() == date.month())
        {
            auto shift = shifter[calendar.dayOfWeek() - 1];
            if (shift == shifter[weakstart - 1])
                pen.setColor(properties.weakstartColour);
            else if (shift == shifter[weakend - 1])
                pen.setColor(properties.weakendColour);
            else
                pen.setColor(properties.weakdayColour);

            painter->setPen(pen);
            painter->drawText(x + (shift * w), y, w, h, textAlignFlags, QString::number(calendar.day()));

            if (shift == shifter[weakend - 1])  //Determine if weakend
                y += h;

            if (y + h >= properties.drawArea.y() + properties.drawArea.height())
                y = properties.drawArea.y();

            calendar = calendar.addDays(1);
        }
    }
}