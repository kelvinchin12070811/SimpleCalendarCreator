/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/SimpleCalendarCreator.hpp"

#include <qdatetime.h>
#include <qmessagebox.h>
#include <qpainter.h>

#include "command/AddObject.hpp"
#include "command/RemoveObject.hpp"
#include "command/ResizeCalendar.hpp"
#include "window/CalendarResizer.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif

const QSize* SimpleCalendarCreator::default_calender_size = new QSize{ 783, 709 };
const QString* SimpleCalendarCreator::window_title = new QString{ "%1 - Simple Calendar Creator" };

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

QSize SimpleCalendarCreator::getCalendarSize() const noexcept
{
    return szCalendar;
}

void SimpleCalendarCreator::setProjectName(const QString& value) noexcept
{
    projectName = value;
    this->setWindowTitle(window_title->arg(projectName));
}

void SimpleCalendarCreator::resizeEvent(QResizeEvent* ev)
{
    ui->winOutline->fitInView(0, 0, static_cast<qreal>(szCalendar.width()),
        static_cast<qreal>(szCalendar.height()), Qt::AspectRatioMode::KeepAspectRatio);
    ui->winOutline->centerOn((szCalendar.width() + 10.0) / 2.0, (szCalendar.height() + 10.0) / 2.0);
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

    onNewProject();
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
    //Clear history stack.
    if (!undoStack.empty())
        undoStack.swap(decltype(undoStack){});

    setProjectName();
    auto tempCmd = std::make_unique<command::ResizeCalendar>(*default_calender_size,
        ui->szCalendarIndicator, &szCalendar);
    tempCmd->execute();

    ui->objectList->clear();
    
    QGraphicsScene* scene{ ui->winOutline->scene() };
    if (scene != nullptr)  //Delete previous scene if exits
    {
        ui->winOutline->setScene(nullptr);
        delete scene;
    }
    scene = new QGraphicsScene;

    scene->setSceneRect(0, 0,
        static_cast<qreal>(szCalendar.width() + 10), static_cast<qreal>(szCalendar.height() + 10));
    ui->winOutline->setScene(scene);

    QPixmap border{ szCalendar };
    border.fill(Qt::GlobalColor::white);

    QPainter painter{ &border };
    QPen pen{ painter.pen() };
    pen.setColor(Qt::GlobalColor::black);
    pen.setWidth(5);
    painter.setPen(pen);

    painter.drawLine(0, 0, szCalendar.width(), 0);
    painter.drawLine(0, 0, 0, szCalendar.height());
    painter.drawLine(0, szCalendar.height(), szCalendar.width(), szCalendar.height());
    painter.drawLine(szCalendar.width(), 0, szCalendar.width(), szCalendar.height());

    QGraphicsPixmapItem* borderItem = new QGraphicsPixmapItem{ border };
    scene->addItem(borderItem);

    if (!this->isHidden())
    {
        this->resize(this->size().width() + 1, this->size().height() + 1);
        this->resize(this->size().width() - 1, this->size().height() - 1);
    }

    unsave = false;
}

void SimpleCalendarCreator::onRemoveObject()
{
    auto act = std::make_unique<command::RemoveObject>(ui->objectList, ui->winOutline);
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
