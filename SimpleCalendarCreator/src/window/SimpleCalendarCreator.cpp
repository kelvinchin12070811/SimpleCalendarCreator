/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/SimpleCalendarCreator.hpp"

#include <sstream>

#include <boost/assert.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <pugixml.hpp>

#include <qdatetime.h>
#include <qdebug.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qpainter.h>

#include <zip.hpp>

#include "command/AddObject.hpp"
#include "command/RemoveObject.hpp"
#include "command/ResizeCalendar.hpp"
#include "command/UndoHistory.hpp"
#include "window/CalendarResizer.hpp"
#include "window/PreviewWindow.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif

const QSize* SimpleCalendarCreator::default_calender_size{ new QSize{ 783, 709 } };
const QString* SimpleCalendarCreator::window_title{ new QString{ "%1 - Simple Calendar Creator" } };
const QString* SimpleCalendarCreator::app_version{ new QString{ "1.0.0" } };
const QString* SimpleCalendarCreator::app_uid{
    new QString{ "io.gitlab.kelvinchin12070811.simplecalendarcreator" }
};
const QString* SimpleCalendarCreator::file_version{ new QString{ "1.0.0"} };

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
    ui->winOutline->centerOn(szCalendar.width() / 2.0, szCalendar.height() / 2.0);
}

void SimpleCalendarCreator::connectObjects()
{
    connect(ui->actionAbout, &QAction::triggered,
        [this]() { QMessageBox::about(this, this->windowTitle(), "Created by Kelvin Chin"); });
    connect(ui->actionAbout_Qt, &QAction::triggered,
        [this]() { QMessageBox::aboutQt(this, this->windowTitle()); });
    connect(ui->actionNew, &QAction::triggered, this, &SimpleCalendarCreator::onNewProject);
    connect(ui->actionQuit, &QAction::triggered, [this]() { this->close(); });
    connect(ui->actionSave, &QAction::triggered, this, &SimpleCalendarCreator::onSaveProject);
    connect(ui->actionSave_As, &QAction::triggered, this, &SimpleCalendarCreator::onSaveProjectAs);
    connect(ui->actionUndo, &QAction::triggered, []() { UndoHistory::getInstance()->pop(); });
    connect(ui->btnAddObject, &QPushButton::clicked, this, &SimpleCalendarCreator::onAddObject);
    connect(ui->btnEditObject, &QPushButton::clicked, [this]() {
        auto itm = this->ui->objectList->currentItem();
        if (itm == nullptr) return;
        static_cast<CustomListWidgetItem*>(itm)->getElement()->edit();
    });
    connect(ui->btnRemoveObject, &QPushButton::clicked, this, &SimpleCalendarCreator::onRemoveObject);
    connect(ui->btnResize, &QPushButton::clicked, this, &SimpleCalendarCreator::onResizeCalendar);
    connect(ui->btnPreview, &QPushButton::clicked, [this]() {
        auto previewWindow = std::make_unique<PreviewWindow>(*ui->objectList, szCalendar);
        previewWindow->exec();
    });
}

void SimpleCalendarCreator::initUi()
{
    QDate today{ QDate::currentDate() };
    ui->spnYear->setValue(today.year());

    onNewProject();
}

void SimpleCalendarCreator::onAddObject()
{
    UndoHistory::getInstance()->push(std::make_unique<command::AddObject>(this, ui->objectList));
}

void SimpleCalendarCreator::onNewProject()
{
    if (UndoHistory::getInstance()->hasUnsave())
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
        static_cast<qreal>(szCalendar.width()), static_cast<qreal>(szCalendar.height()));
    ui->winOutline->setScene(scene);

    QPixmap border{ szCalendar };
    border.fill(Qt::GlobalColor::white);

    QGraphicsPixmapItem* borderItem = new QGraphicsPixmapItem{ border };
    scene->addItem(borderItem);

    if (!this->isHidden())
    {
        this->resize(this->size().width() + 1, this->size().height() + 1);
        this->resize(this->size().width() - 1, this->size().height() - 1);
    }

    UndoHistory::getInstance()->changesSaved();
}

void SimpleCalendarCreator::onRemoveObject()
{
    auto undoHistory = UndoHistory::getInstance();
    undoHistory->push(std::make_unique<command::RemoveObject>(ui->objectList, ui->winOutline));
}

void SimpleCalendarCreator::onResizeCalendar()
{
    auto dialog = std::make_unique<CalendarResizer>(szCalendar);
    dialog->exec();

    if (dialog->isAccepted())
    {
        auto cmd = std::make_unique<command::ResizeCalendar>(dialog->getDecidedSize(),
            ui->szCalendarIndicator, &szCalendar);

        UndoHistory::getInstance()->push(std::move(cmd));
    }
}

void SimpleCalendarCreator::onSaveProject()
{
    if (!UndoHistory::getInstance()->hasUnsave()) return;
    if (savedPath.path().isEmpty())
    {
        onSaveProjectAs();
        return;
    }

    std::unique_ptr<libzip::archive> container{ nullptr };
    boost::property_tree::ptree metaIni;
    
    try
    {
        container = std::make_unique<libzip::archive>(savedPath.filePath().toStdString());
    }
    catch (const std::exception& e)
    {
#ifdef _DEBUG
        qDebug() << e.what();
#endif // _DEBUG
        BOOST_ASSERT_MSG(false, "failed to open file");
    }

    try
    {
        libzip::stat fileId;
        fileId = container->stat("_meta/meta.ini");
        libzip::file file{ container->open(fileId.index) };
        std::istringstream ss(file.read(fileId.size));
        boost::property_tree::ini_parser::read_ini(ss, metaIni);
    }
    catch (const std::runtime_error & e)
    {
        QMessageBox::critical(this, "Error on saving file", e.what());
    }

    QString appId{ metaIni.get<char*>("app.uid") };
    if (appId != *SimpleCalendarCreator::app_uid)
    {
        QMessageBox::critical(this, "Unsupported File Format",
            "This file is not created for Simple Calendar Creator");
        return;
    }

    QString verFile{ metaIni.get<char*>("file.version") };
    QString verApp{ metaIni.get<char*>("app.version") };
    if (*SimpleCalendarCreator::file_version < verFile ||
        *SimpleCalendarCreator::app_version < verApp)
    {
        QMessageBox::critical(this, "Unsupported File Format",
            "This file is created for newer version of Simple Calendar Creator which is not supported.");
        return;
    }

    QString modTime{ QDateTime::currentDateTimeUtc().toString(Qt::DateFormat::ISODate) };
    metaIni.put("file.created", modTime.toStdString());

    pugi::xml_document document;
    auto design = document.append_child("design");
    for (int idx{ 0 }; idx < ui->objectList->count(); idx++)
    {
        auto item = static_cast<CustomListWidgetItem*>(ui->objectList->item(idx));
        auto objectNode = design.append_child("calendar_obj");
        objectNode.append_attribute("name").set_value(item->text().toUtf8().data());
        item->getElement()->serialize(&objectNode);
    }

    auto saveWorker = [&container](const std::string & data, const std::string & file) {
        try
        {
            auto stat = container->stat(file);
            container->replace(libzip::source_buffer(data), stat.index);
        }
        catch (const std::runtime_error & e)
        {
            container->add(libzip::source_buffer(data), file);
        }
    };

    std::ostringstream ss;
    boost::property_tree::ini_parser::write_ini(ss, metaIni);
    saveWorker(ss.str(), "_meta/meta.ini");

    ss.swap(std::ostringstream{});
    document.save(ss, "    ");
    saveWorker(ss.str(), "design.xml");
}

void SimpleCalendarCreator::onSaveProjectAs()
{
    if (!UndoHistory::getInstance()->hasUnsave()) return;

    QString path{ QFileDialog::getSaveFileName(this, "Save as", "", "Calendar design(*.calendar)") };
#ifdef _DEBUG
    qDebug() << "output path: " << path;
#endif // _DEBUG

    if (path.isEmpty()) return;

    QString modTime{ QDateTime::currentDateTimeUtc().toString(Qt::DateFormat::ISODate) };

    boost::property_tree::ptree meta;
    meta.add("spec.version", "1.0.0");
    meta.add("app.uid", SimpleCalendarCreator::app_uid->toStdString());
    meta.add("app.version", SimpleCalendarCreator::app_version->toStdString());
    meta.add("file.version", SimpleCalendarCreator::file_version->toStdString());
    meta.add("file.created", modTime.toStdString());
    meta.add("file.modified", modTime.toStdString());

    pugi::xml_document document;
    auto design = document.append_child("design");
    for (int idx{ 0 }; idx < ui->objectList->count(); idx++)
    {
        auto item = dynamic_cast<CustomListWidgetItem*>(ui->objectList->item(idx));
        BOOST_ASSERT_MSG(item != nullptr, "item is not CustomListWidget");
        auto objectNode = design.append_child("calendar_obj");
        objectNode.append_attribute("name").set_value(item->text().toUtf8().data());
        item->getElement()->serialize(&objectNode);
    }

    libzip::archive output{ path.toStdString(), ZIP_CREATE };

    auto writeOutput = [&output](const std::string& filename, const libzip::source& source) {
        try
        {
            auto info = output.stat(filename);
            output.replace(source, info.index);
        }
        catch (const std::runtime_error&)
        {
            output.add(source, filename);
        }
    };

    std::ostringstream buffer;
    boost::property_tree::ini_parser::write_ini(buffer, meta);
    writeOutput("_meta/meta.ini", libzip::source_buffer(buffer.str()));

    buffer.swap(std::ostringstream{});
    document.save(buffer, "    ");
    writeOutput("design.xml", libzip::source_buffer(buffer.str()));

    savedPath = path;
    setProjectName(savedPath.completeBaseName());
}
