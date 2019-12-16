/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "command/object_properties_modifier/LineObject.hpp"

namespace command::object_properties_modifier
{
    LineObject::LineObject(element::object_properties::Line* properties,
      const element::object_properties::Line& newValue):
        prevProperties(*properties), newProperties(newValue), curnProperties(properties)
    {
        assert(properties != nullptr);
    }
    
    bool LineObject::execute()
    {
        *curnProperties = newProperties;
        emit propertiesChanged();
        return true;
    }
    
    void LineObject::unexecute()
    {
        *curnProperties = prevProperties;
        emit propertiesChanged();
    }
}