/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include "ui_SimpleCalendarCreator.h"

#include <memory>
#include <stack>

#include <QtWidgets/QMainWindow>

#include "command/Command.hpp"

/**
 * @brief Main window of the program. Initialized by the main entry point.
 */
class SimpleCalendarCreator : public QMainWindow
{
    Q_OBJECT
public:
    static const QSize* default_calender_size;  /**< @brief default size of the calendar design. */
    static const QString* window_title;  /**< @brief Window title text. */
public:
    SimpleCalendarCreator(QWidget *parent = Q_NULLPTR);
    /**
     * @brief Get ui elements of the MainWindow.
     */
    const Ui::SimpleCalendarCreatorClass* getUi() const noexcept;

public: //Getters
    /**
     * @brief Get the size of calendar design.
     */
    QSize getCalendarSize() const noexcept;

public:  //Setters
    /**
     * @brief Set the project name and update the window title.
     */
    void setProjectName(const QString& value = "Untitled") noexcept;

protected:
    /**
     * @internal
     * @brief Event handeling function when the window is resized.
     */
    void resizeEvent(QResizeEvent* ev) override;

private:
    void connectObjects();
    void initUi();
    /**
     * @internal
     * @brief Push a command to the undo stack.
     */
    void pushUndoSatck(std::unique_ptr<command::Command> cmd);

private: //slots
    /**
     * @internal
     * @brief Slot when undo action is trigred.
     */
    void onActionUndo();
    /**
     * @internal
     * @brief Slot when a calendar object is being to add to the design.
     */
    void onAddObject();
    /**
     * @internal
     * @brief Slot when user create new project.
     */
    void onNewProject();
    /**
     * @internal
     * @brief Slot when a calendar object is being to remove from the design.
     */
    void onRemoveObject();
    /**
     * @internal
     * @brief Slot when user decide to resize their calendar design.
     */
    void onResizeCalendar();

private:
    /**
     * @internal
     * @brief Determine if the user have unsave work.
     */
    bool unsave{ false };
    /**
     * @internal
     * @brief History tracking stack that holds previous executed command.
     */
    std::stack<std::unique_ptr<command::Command>> undoStack;
    /**
     * @internal
     * @brief Ui elements of the window.
     */
    std::unique_ptr<Ui::SimpleCalendarCreatorClass> ui{ nullptr };

    /**
     * @internal
     * @brief Size of calendar.
     */
    QSize szCalendar{ *SimpleCalendarCreator::default_calender_size };
    /**
     * @internal
     * @brief Name of the project, "Untitled" by default.
     */
    QString projectName;
};
