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
        BOOST_ASSERT_MSG(false, "unimplemented function");
        return QPixmap();
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
        BOOST_ASSERT_MSG(false, "unimplemented function");
    }
    
    void MonthTitle::deserialize(const pugi::xml_node& node)
    {
        BOOST_ASSERT_MSG(false, "unimplemented function");
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

        QString monthName{ properties.locale.toString(date, "MMMM") };
        painter->drawText(properties.pos, monthName);
    }
}