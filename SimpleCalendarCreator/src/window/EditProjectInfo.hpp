/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <functional>
#include <memory>

#include <QDialog>

#include "ui_EditProjectInfo.h"
#include "window/SimpleCalendarCreator.hpp"

/**
 * @brief Dialogue that allow user to modify Project info such as size and targeted year.
 */
class EditProjectInfo : public QDialog
{
    Q_OBJECT
public:
    /** Templated string that display the selected year to generate calendar. */
    static constexpr char* const format_targeted_year{ "Year: %1" };
    /**
     * Templated string that display the size of the calendar.
     */
    static constexpr char* const format_calendar_size{ "Calendar size: %1x%2 px" };
public:
    /**
     * Create new dialogue.
     * @param properties Properties of calendar, must not be nullptr.
     * @param slot Slot to call when properties changed via UndoHistory Command, must not be nullptr.
     * @param parent Parent of dialogue, nullpter if no parent. Default as nullptr.
     */
    EditProjectInfo(CalendarProperties* properties, std::function<void()> slot, QWidget *parent = Q_NULLPTR);
    ~EditProjectInfo() noexcept = default;
private:
    /**
     * @internal
     * Connect components to apporpaite slot.
     */
    void connectObjects();
    /**
     * @internal
     * Additional steps to initialize UI.
     */
    void initUi();

private slots:
    /**
     * @internal
     * Slot called when ok button is clicked.
     */
    void onAccepted();

private:
    /**
     * @internal
     * Properties of calendar, must not be nullptr.
     */
    CalendarProperties* properties{ nullptr };
    /**
     * @internal
     * UI component of dialogue.
     */
    std::unique_ptr<Ui::EditProjectInfo> ui{ nullptr };
    /**
     * @internal
     * Slot to forward to ChangeObjectProperties command, must not be nullptr.
     */
    std::function<void()> propertiesChangedSlot{ nullptr };
};
