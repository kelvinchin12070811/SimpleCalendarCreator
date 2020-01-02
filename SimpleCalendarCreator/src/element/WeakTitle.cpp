/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/WeakTitle.hpp"

#include <boost/assert.hpp>

#include <QDate>
#include <QPainter>

#include "element/CustomListWidgetItem.hpp"
#include "window/object_editor/EditWeakTitle.hpp"

namespace element
{
    WeakTitle::WeakTitle()
    {
        properties = {
            false,
            1,
            Qt::GlobalColor::black,
            Qt::GlobalColor::blue,
            Qt::GlobalColor::red,
            QFont{},
            QRect{},
            {}
        };
    }
    
    void WeakTitle::setParent(CustomListWidgetItem* parent)
    {
        BOOST_ASSERT_MSG(parent != nullptr, "parent must not be nullptr");
        this->parent = parent;
    }

    void WeakTitle::setSize(const QSize& size)
    {
        if (graphic.isNull())
            graphic = QPixmap{ size };
        else
            graphic.scaled(size);
    }
    
    const QPixmap& WeakTitle::getRenderedGraphics()
    {
        return graphic;
    }
    
    QPixmap WeakTitle::render(const QDate& date)
    {
        QPixmap rendered{ graphic.size() };
        rendered.fill(Qt::GlobalColor::transparent);

        QPainter painter{ &rendered };
        painter.setRenderHint(QPainter::RenderHint::TextAntialiasing);
        drawTitle(&painter, QDate{ QDate::currentDate().year(), 1, 1 });
        return rendered;
    }
    
    void WeakTitle::edit(QWidget* parent)
    {
        auto dialog = std::make_unique<EditWeakTitle>(&properties, parent);
        QString title{ dialog->windowTitle().arg(this->parent->text()) };
        dialog->setWindowTitle(title);
        dialog->forwardConnect(std::bind(&WeakTitle::drawOutline, this));
        dialog->exec();
    }
    
    void WeakTitle::serialize(pugi::xml_node* node)
    {
        BOOST_ASSERT_MSG(node != nullptr, "node must not be nullptr");
        node->append_attribute("type").set_value(Element::getTypeName<WeakTitle>().c_str());
        
        auto nodColour = node->append_child("colour");
        nodColour.append_child("weakday").text().set(properties.normalTextColour
            .name(QColor::NameFormat::HexArgb).toStdString().c_str());
        nodColour.append_child("weakend").text().set(properties.satTextColour
            .name(QColor::NameFormat::HexArgb).toStdString().c_str());
        nodColour.append_child("weakstart").text().set(properties.sunTextColour
            .name(QColor::NameFormat::HexArgb).toStdString().c_str());
        
        node->append_child("font").text().set(properties.font.toString().toStdString().c_str());
        
        auto nodBound = node->append_child("label-bound");
        nodBound.append_attribute("x").set_value(properties.fontRect.x());
        nodBound.append_attribute("y").set_value(properties.fontRect.y());
        nodBound.append_attribute("w").set_value(properties.fontRect.width());
        nodBound.append_attribute("h").set_value(properties.fontRect.height());

        auto nodText = node->append_child("text");
        nodText.append_attribute("vertical").set_value(properties.isVertical);
        nodText.append_attribute("text-alignment").set_value(properties.textAlignment);
        for (const auto& itr : properties.lables)
        {
            auto child = nodText.append_child("item-group");
            child.append_attribute("name").set_value(itr.first.toStdString().c_str());
            for (const auto& itr2 : itr.second)
                child.append_child("item").text().set(itr2.toStdString().c_str());
        }
    }
    
    void WeakTitle::deserialize(const pugi::xml_node& node)
    {
        auto nodColour = node.child("colour");
        properties.normalTextColour = QColor{ nodColour.child("weakday").text().as_string() };
        properties.satTextColour = QColor{ nodColour.child("weakend").text().as_string() };
        properties.sunTextColour = QColor{ nodColour.child("weakstart").text().as_string() };

        properties.font.fromString(node.child("font").text().as_string());

        auto nodTextBound = node.child("label-bound");
        properties.fontRect = QRect{
            nodTextBound.attribute("x").as_int(),
            nodTextBound.attribute("y").as_int(),
            nodTextBound.attribute("w").as_int(),
            nodTextBound.attribute("h").as_int()
        };

        auto nodText = node.child("text");
        properties.isVertical = nodText.attribute("vertical").as_bool();
        properties.textAlignment = static_cast<uint8_t>(nodText.attribute("text-alignment").as_uint(1));

        auto nodItemGroup = nodText.children("item-group");
        for (const auto& itr : nodItemGroup)
        {
            QString name{ itr.attribute("name").as_string() };
            std::vector<QString> labels;
            labels.reserve(7);
            for (const auto& itr2 : itr.children("item"))
                labels.emplace_back(itr2.text().as_string());

            properties.lables.emplace_back(std::move(name), std::move(labels));
        }
        properties.lables.shrink_to_fit();
        drawOutline();
    }
    
    void WeakTitle::drawOutline()
    {
        graphic.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &graphic };
        QDate date{ QDate::currentDate().year(), 1, 1 };
        painter.fillRect(properties.fontRect, QColor{ WeakTitle::outline_background_colour });
        drawTitle(&painter, date);
        parent->renderOutline();
    }

    void WeakTitle::drawTitle(QPainter* painter, const QDate& date)
    {
        painter->setFont(properties.font);
        QPen pen;
        int idxToDraw{ std::clamp(date.month() - 1, 0, 11) };
        if (idxToDraw >= properties.lables.size())
            idxToDraw = idxToDraw % properties.lables.size();

        QRect labelRect = properties.fontRect;
        labelRect.setWidth(labelRect.width() / 7);
        for (const auto& itr : properties.lables[idxToDraw].second)
        {
            if (labelRect.x() >= properties.fontRect.x() + (labelRect.width() * 6))
                pen.setColor(properties.satTextColour);
            else if (labelRect.x() >= properties.fontRect.x() + (labelRect.width() * 1))
                pen.setColor(properties.normalTextColour);
            else
                pen.setColor(properties.sunTextColour);

            int flags{ Qt::AlignmentFlag::AlignVCenter };
            switch (properties.textAlignment)
            {
            case 2:
                flags |= Qt::AlignmentFlag::AlignHCenter;
                break;
            case 3:
                flags |= Qt::AlignmentFlag::AlignRight;
                break;
            default:
                flags |= Qt::AlignmentFlag::AlignLeft;
                break;
            }

            painter->setPen(pen);
            painter->drawText(labelRect, flags, itr);
            labelRect = QRect{
                labelRect.x() + labelRect.width(),
                labelRect.y(),
                labelRect.width(),
                labelRect.height()
            };
        }
    }
}