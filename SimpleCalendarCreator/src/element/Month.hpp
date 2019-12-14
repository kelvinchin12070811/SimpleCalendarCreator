/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <cassert>

namespace element
{
    /**
     * @brief Month definetions
     */
    enum class Month
    {
        january = 0,
        febuary,
        march,
        apirl,
        may,
        june,
        july,
        august,
        september,
        october,
        november,
        december
    };

    const char* monthToString(const Month& month);
    Month stringToMonth(const char* string);
}