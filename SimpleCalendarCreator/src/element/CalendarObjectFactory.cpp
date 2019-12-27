/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/CalendarObjectFactory.hpp"

#include "element/Ellipse.hpp"
#include "element/Line.hpp"
#include "element/Rectangle.hpp"
#include "element/Text.hpp"

const std::map<QString, std::function<std::unique_ptr<element::Element>()>>
    *CalendarObjectFactory::objCreator{
        new std::map<QString, std::function<std::unique_ptr<element::Element>()>>{
            {
                QString::fromStdString(element::Element::getTypeName<element::Ellipse>()),
                []() { return std::make_unique<element::Ellipse>(); }
            },
            {
                QString::fromStdString(element::Element::getTypeName<element::Line>()),
                []() { return std::make_unique<element::Line>(); }
            },
            {
                QString::fromStdString(element::Element::getTypeName<element::Rectangle>()),
                []() { return std::make_unique<element::Rectangle>(); }
            },
            {
                QString::fromStdString(element::Element::getTypeName<element::Text>()),
                []() { return std::make_unique<element::Text>(); }
            }
        }
    };

const std::map<QString, QString> *CalendarObjectFactory::objNativeName =
    new std::map<QString, QString>{
        { "Ellipse", QString::fromStdString(element::Element::getTypeName<element::Ellipse>()) },
        { "Line", QString::fromStdString(element::Element::getTypeName<element::Line>()) },
        { "Rectangle", QString::fromStdString(element::Element::getTypeName<element::Rectangle>()) },
        { "Text", QString::fromStdString(element::Element::getTypeName<element::Text>()) }
    };

std::unique_ptr<element::Element> CalendarObjectFactory::createObject(const QString& name) const
{
    try
    {
        return objCreator->at(name)();
    }
    catch (const std::out_of_range & e)
    {
        throw std::out_of_range{ QString{ "The object \"%1\" has not yet been registered." }.arg(name)
            .toStdString() };
    }
}

QString CalendarObjectFactory::getObjectClassName(const QString& name) const
{
    try
    {
        return objNativeName->at(name);
    }
    catch (const std::out_of_range & e)
    {
        throw std::out_of_range{ QString{ "The object \"%1\" has not yet been registered." }.arg(name)
        .toStdString() };
    }
}

std::vector<QString> CalendarObjectFactory::getObjectReadableName() const noexcept
{
    std::vector<QString> list;
    list.reserve(objNativeName->size());
    for (auto itr : *objNativeName)
    {
        list.push_back(itr.first);
    }

    return list;
}
