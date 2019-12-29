/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/TemplatedText.hpp"

#include <algorithm>

#include <boost/assert.hpp>

#include <QTextBlock>
#include <QTextDocument>

#include "element/CustomListWidgetItem.hpp"
#include "element/Text.hpp"
#include "window/object_editor/EditTemplatedText.hpp"

namespace element
{
    TemplatedText::TemplatedText()
    {
        properties = {
            false,
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
            graphic.scaled(size, Qt::AspectRatioMode::KeepAspectRatio);
    }
    
    const QPixmap& TemplatedText::getRenderedGraphics()
    {
        return graphic;
    }
    
    QPixmap TemplatedText::render(const Month& month)
    {
        QPixmap rendered{ graphic.size() };
        rendered.fill(Qt::GlobalColor::transparent);

        QPainter painter{ &rendered };
        painter.setRenderHint(QPainter::RenderHint::TextAntialiasing);
        drawText(&painter, month);
        return rendered;
    }
    
    void TemplatedText::edit()
    {
        auto dialog = std::make_unique<EditTemplatedText>(&properties);
        QString title{ dialog->windowTitle().arg(parent->text()) };

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
        drawText(&painter);
        parent->renderOutline();
    }

    void TemplatedText::drawText(QPainter* painter, const Month& month)
    {
        BOOST_ASSERT_MSG(painter != nullptr, "painter must not be nullptr");
        int idx{ std::clamp(static_cast<int>(month), static_cast<int>(Month::january),
            static_cast<int>(Month::december)) };
        if (idx >= properties.texts.size())
            idx = idx % properties.texts.size();

        QString textToRender{ properties.texts.at(idx) };
        QString html;
        if (properties.isVertical)
        {
            for (auto itr : textToRender)
                html += QString{ "%1<br>" }.arg(itr);
        }
        else
        {
            html = textToRender;
        }

        QTextDocument document;
        document.setDefaultFont(properties.font);
        document.setHtml(QString{ Text::css_rules }.arg(html).arg(properties.textColour
            .name(QColor::NameFormat::HexArgb)));

        for (auto itr = document.begin(); itr != document.end(); itr = itr.next())
        {
            QTextCursor tc{ itr };
            auto format = itr.blockFormat();
            format.setLineHeight(Text::line_height, QTextBlockFormat::LineHeightTypes::ProportionalHeight);
            tc.setBlockFormat(format);
        }

        painter->translate(properties.pos);
        document.drawContents(painter);
    }
}