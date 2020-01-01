/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/Dates.hpp"

#include <QDate>

#include "element/CustomListWidgetItem.hpp"
#include "window/object_editor/EditDates.hpp"

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
        BOOST_ASSERT_MSG(false, "function unimplemented");
        return QPixmap();
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
        BOOST_ASSERT_MSG(false, "function unimplemented");
    }
    
    void Dates::deserialize(const pugi::xml_node& node)
    {
        BOOST_ASSERT_MSG(false, "unimplemented");
    }
    
    void Dates::drawOutline()
    {
        graphic.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &graphic };
        QDate date{ QDate::currentDate().year(), 1, 1 };
        drawLabels(&painter, date);
        parent->renderOutline();
    }
    
    void Dates::drawLabels(QPainter* painter, const QDate& date)
    {
        BOOST_ASSERT_MSG(false, "unimplemented");
    }
}