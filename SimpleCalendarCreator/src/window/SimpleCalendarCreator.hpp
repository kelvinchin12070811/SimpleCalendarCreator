/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include "ui_SimpleCalendarCreator.h"

#include <memory>
#include <stack>

#include <qfileinfo.h>
#include <QtWidgets/QMainWindow>

#include "command/Command.hpp"

/**
 * @brief Main window of the program. Initialized by the main entry point.
 */
class SimpleCalendarCreator : public QMainWindow
{
    Q_OBJECT
public:
    /** @brief default size of the calendar design. */
    static constexpr QSize default_calender_size{ 783, 709 };
    /** @brief Window title text. */
    static constexpr std::string_view window_title{ "%1 - Simple Calendar Creator" };
    /** @brief Version num of the app, in major.minor.bugfix format. */
    static constexpr std::string_view app_version{ "1.0.0" };
    /** @brief Uid of the applab project. */
    static constexpr std::string_view app_uid{ "io.gitlab.kelvinchin12070811.simplecalendarcreator" };
    /** @brief Version of the file format, in major.minor.bugfix format.*/
    static constexpr std::string_view file_version{ "1.0.0" };
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

private: //slots
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
     * @brief Slot when user open existing project.
     */
    void onOpenProject();
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
    /**
     * @internal
     * @brief Slot when trying to save a project.
     */
    void onSaveProject();
    /**
     * @internal
     * @brief Slot when trying to save project as.
     */
    void onSaveProjectAs();

private:
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
     * @brief Path of last saved file. Empty to trigger save as.
     */
    QFileInfo savedPath;
    /**
     * @internal
     * @brief Size of calendar.
     */
    QSize szCalendar{ SimpleCalendarCreator::default_calender_size };
    /**
     * @internal
     * @brief Name of the project, "Untitled" by default.
     */
    QString projectName;
};
