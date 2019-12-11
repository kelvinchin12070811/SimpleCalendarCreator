/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <qlabel.h>
#include <qstring.h>

#include <command/Command.hpp>

namespace command
{
    /**
     * @brief Command to resize the calender design.
     */
    class ResizeCalendar : public command::Command
    {
    public:
        static const QString* sz_indicator_text;
    public:
        /**
         * @brief Create new ResizeCalender command.
         * @param newSize Target size to resize.
         * @param szIndicator Indicator that tell the user what ther design size is, this will modify the
         * text shown on the indicator. Can't be nullptr.
         * @param curSize Size of the calendar, this will resize the calendar when it execute. Can't be
         * nullptr.
         */
        ResizeCalendar(const QSize& newSize, QLabel* szIndicator, QSize* curSize);
        bool execute() override;
        void unexecute() override;
    private:
        /**
         * @internal
         * @brief Reference to size indicator which tell user the size of their calendar design.
         */
        QLabel* szIndicator{ nullptr };
        /**
         * @internal
         * @brief Target size to resize.
         */
        QSize newSize;
        /**
         * @internal
         * @brief Calendar size before resize.
         */
        QSize prevSize;
        /**
         * @internal
         * @brief Reference to the master calendar design size.
         */
        QSize* size{ nullptr };
    };
}
