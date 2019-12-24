/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <qlistwidget.h>
#include <qstring.h>

#include "element/Element.hpp"

/**
 * @brief Factory that construct calendar object via string.
 */
class CalendarObjectFactory
{
public:
    /**
     * Create object from class name.
     * @param name Class name of the object.
     * @throw std::out_of_range if @p name is not registered.
     */
    std::unique_ptr<element::Element> createObject(const QString& name) const;
    /**
     * Get calendar object's class name from readable name.
     * @param name Readable name of a calendar object.
     * @throw std::out_of_range if @p name is not registered.
     */
    QString getObjectClassName(const QString& name) const;
    /**
     * Get all avaliable calendar objects' readable name.
     */
    std::vector<QString> getObjectReadableName() const noexcept;

private:
    /**
     * @internal
     * Map Calendar Objects' class name to relative creator function.
     */
    static const std::map<QString, std::function<std::unique_ptr<element::Element>()>>* objCreator;
    /**
     * @internal
     * Map Calendar Objects' readable name to class name.
     */
    static const std::map<QString, QString>* objNativeName;
};