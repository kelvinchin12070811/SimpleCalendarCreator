/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/Month.hpp"

#include <algorithm>
#include <string>

namespace element
{
    const char* monthToString(const Month& month)
    {
        switch (month)
        {
        case Month::january:
            return "January";
            break;
        case Month::febuary:
            return "Febuary";
            break;
        case Month::march:
            return "March";
            break;
        case Month::apirl:
            return "Apirl";
            break;
        case Month::may:
            return "May";
            break;
        case Month::june:
            return "June";
            break;
        case Month::july:
            return "July";
            break;
        case Month::august:
            return "August";
            break;
        case Month::september:
            return "September";
            break;
        case Month::october:
            return "October";
            break;
        case Month::november:
            return "November";
            break;
        case Month::december:
            return "December";
            break;
        default:
            assert(false);
            break;
        }
        return "";
    }
    
    Month stringToMonth(const char* string)
    {
        std::string monthName{ string };
        std::transform(monthName.begin(), monthName.end(), monthName.begin(), [](auto c) {
            return std::tolower(c);
        });

        monthName[0] = std::toupper(static_cast<int>(monthName[0]));

        Month month{ Month::january };
        for (int i{ 0 }; i < 12; i++)
        {
            month = static_cast<Month>(i);
            if (monthName == element::monthToString(month))
                break;
        }
        return month;
    }
}