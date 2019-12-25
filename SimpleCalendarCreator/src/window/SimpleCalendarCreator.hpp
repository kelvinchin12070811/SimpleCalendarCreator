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

#include <zip.hpp>

#include "command/Command.hpp"

/**
 * @brief Main window of the program. Initialized by the main entry point.
 */
class SimpleCalendarCreator : public QMainWindow
{
    Q_OBJECT
public:
    /** default size of the calendar design. */
    static constexpr QSize default_calender_size{ 783, 709 };
    /** Window title text. */
    static constexpr std::string_view window_title{ "%1 - Simple Calendar Creator" };
    /** Version num of the app, in major.minor.bugfix format. */
    static constexpr std::string_view app_version{ "1.0.0" };
    /** Uid of the applab project. */
    static constexpr std::string_view app_uid{ "io.gitlab.kelvinchin12070811.simplecalendarcreator" };
    /** Version of the file format, in major.minor.bugfix format.*/
    static constexpr std::string_view file_version{ "1.0.0" };
public:
    SimpleCalendarCreator(QWidget *parent = Q_NULLPTR);
    /**
     * Get ui elements of the MainWindow.
     */
    const Ui::SimpleCalendarCreatorClass* getUi() const noexcept;

public: //Getters
    /**
     * Get the size of calendar design.
     */
    QSize getCalendarSize() const noexcept;

public:  //Setters
    /**
     * Set the project name and update the window title.
     */
    void setProjectName(const QString& value = "Untitled") noexcept;

protected:
    /**
     * @internal
     * Event handeling function when the window is resized.
     */
    void resizeEvent(QResizeEvent* ev) override;

private:
    /**
     * @internal
     * Connect each components to slot.
     */
    void connectObjects();
    /**
     * @internal
     * Additional steps to initialize UI.
     */
    void initUi();
    /**
     * @internal
     * General algorithm to save file. Save file will only update modified date while save as will update
     * everything.
     * @param path Path to save project.
     * @param createdTime Time when the file is created. Empty for not created yet.
     */
    void saveWorker(const QString& path, const QString& createdTime = QString{});

private: //slots
    /**
     * @internal
     * Slot when a calendar object is being to add to the design.
     */
    void onAddObject();
    /**
     * @internal
     * Slot when user create new project.
     */
    void onNewProject();
    /**
     * @internal
     * Slot when user open existing project.
     */
    void onOpenProject();
    /**
     * @internal
     * Slot when a calendar object is being to remove from the design.
     */
    void onRemoveObject();
    /**
     * @internal
     * Slot when user decide to resize their calendar design.
     */
    void onResizeCalendar();
    /**
     * @internal
     * Slot when trying to save a project.
     */
    void onSaveProject();
    /**
     * @internal
     * Slot when trying to save project as.
     */
    void onSaveProjectAs();

private:
    /**
     * @internal
     * Ui elements of the window.
     */
    std::unique_ptr<Ui::SimpleCalendarCreatorClass> ui{ nullptr };

    /**
     * @internal
     * Path of last saved file. Empty to trigger save as.
     */
    QFileInfo savedPath;
    /**
     * @internal
     * Size of calendar.
     */
    QSize szCalendar{ SimpleCalendarCreator::default_calender_size };
    /**
     * @internal
     * Name of the project, "Untitled" by default.
     */
    QString projectName;
};
