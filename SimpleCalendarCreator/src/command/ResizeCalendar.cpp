/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "command/ResizeCalendar.hpp"

namespace command
{
    const QString* ResizeCalendar::sz_indicator_text = new QString{ "Calendar size: %1x%2 px" };
    
    ResizeCalendar::ResizeCalendar(const QSize& newSize, QLabel* szIndicator, QSize* curSize):
        newSize(newSize), prevSize(*curSize), szIndicator(szIndicator), size(curSize)
    {
        assert(szIndicator != nullptr);
        assert(curSize != nullptr);
    }
    
    bool ResizeCalendar::execute()
    {
        size->setWidth(newSize.width());
        size->setHeight(newSize.height());

        szIndicator->setText(sz_indicator_text->arg(newSize.width()).arg(newSize.height()));
        return true;
    }
    
    void ResizeCalendar::unexecute()
    {
        size->setWidth(prevSize.width());
        size->setHeight(prevSize.height());

        szIndicator->setText(sz_indicator_text->arg(prevSize.width()).arg(prevSize.height()));
    }
}
