/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/SimpleCalendarCreator.hpp"

#include <qdatetime.h>
#include <qmessagebox.h>

#include "command/AddObject.hpp"
#include "command/RemoveObject.hpp"
#include "command/ResizeCalendar.hpp"
#include "window/CalendarResizer.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif

const QSize* SimpleCalendarCreator::default_calender_size = new QSize{ 783, 709 };

SimpleCalendarCreator::SimpleCalendarCreator(QWidget *parent)
    : QMainWindow(parent), ui(std::make_unique<Ui::SimpleCalendarCreatorClass>())
{
    ui->setupUi(this);
    connectObjects();
    initUi();
}

const Ui::SimpleCalendarCreatorClass* SimpleCalendarCreator::getUi() const noexcept
{
    return ui.get();
}

void SimpleCalendarCreator::connectObjects()
{
    connect(ui->actionAbout, &QAction::triggered,
        [this]() { QMessageBox::about(this, this->windowTitle(), "Created by Kelvin Chin"); });
    connect(ui->actionAbout_Qt, &QAction::triggered,
        [this]() { QMessageBox::aboutQt(this, this->windowTitle()); });
    connect(ui->actionNew, &QAction::triggered, this, &SimpleCalendarCreator::onNewProject);
    connect(ui->actionQuit, &QAction::triggered, [this]() { this->close(); });
    connect(ui->actionUndo, &QAction::triggered, this, &SimpleCalendarCreator::onActionUndo);
    connect(ui->btnAddObject, &QPushButton::clicked, this, &SimpleCalendarCreator::onAddObject);
    connect(ui->btnEditObject, &QPushButton::clicked, [this]() {
        auto itm = this->ui->objectList->currentItem();
        if (itm == nullptr) return;
        static_cast<CustomListWidgetItem*>(itm)->getElement()->edit();
    });
    connect(ui->btnRemoveObject, &QPushButton::clicked, this, &SimpleCalendarCreator::onRemoveObject);
    connect(ui->btnResize, &QPushButton::clicked, this, &SimpleCalendarCreator::onResizeCalendar);
}

void SimpleCalendarCreator::initUi()
{
    QDate today{ QDate::currentDate() };
    ui->spnYear->setValue(today.year());

    ui->szCalendarIndicator->setText(
        command::ResizeCalendar::sz_indicator_text->arg(szCalendar.width()).arg(szCalendar.height()));
}

void SimpleCalendarCreator::pushUndoSatck(std::unique_ptr<command::Command> cmd)
{
    undoStack.push(std::move(cmd));
    unsave = true;
}

void SimpleCalendarCreator::onActionUndo()
{
    if (undoStack.empty()) return;
    undoStack.top()->unexecute();
    undoStack.pop();
    unsave = false;
}

void SimpleCalendarCreator::onAddObject()
{
    auto act = std::make_unique<command::AddObject>(this, ui->objectList);
    if (!act->execute()) return;
    pushUndoSatck(std::move(act));
}

void SimpleCalendarCreator::onNewProject()
{
    if (unsave == true)
    {
        auto result = QMessageBox::information(this, "Unsaved work", QString{ "Operation will cause" }
            + " unsaved work lost, are your sure to continue?", QMessageBox::Yes | QMessageBox::No);

        if (result == QMessageBox::No) return;
    }
    undoStack.swap(decltype(undoStack){});
    auto tempCmd = std::make_unique<command::ResizeCalendar>(*default_calender_size, ui->szCalendarIndicator,
        &szCalendar);
    tempCmd->execute();
    ui->objectList->clear();
    unsave = false;
}

void SimpleCalendarCreator::onRemoveObject()
{
    auto act = std::make_unique<command::RemoveObject>(ui->objectList);
    if (!act->execute()) return;
    pushUndoSatck(std::move(act));
}

void SimpleCalendarCreator::onResizeCalendar()
{
    auto dialog = std::make_unique<CalendarResizer>(szCalendar);
    dialog->exec();

    if (dialog->isAccepted())
    {
        auto cmd = std::make_unique<command::ResizeCalendar>(dialog->getDecidedSize(),
            ui->szCalendarIndicator, &szCalendar);

        if (!cmd->execute()) return;
        pushUndoSatck(std::move(cmd));
    }
}
