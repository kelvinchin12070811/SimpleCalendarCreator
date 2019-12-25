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

#include "command/AddObject.hpp"
#include "command/RemoveObject.hpp"
#include "command/ResizeCalendar.hpp"
#include "command/UndoHistory.hpp"
#include "element/CalendarObjectFactory.hpp"
#include "window/CalendarResizer.hpp"
#include "window/PreviewWindow.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif

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
    this->setWindowTitle(QString{ SimpleCalendarCreator::window_title.data() }.arg(value));
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
    connect(ui->actionOpen, &QAction::triggered, this, &SimpleCalendarCreator::onOpenProject);
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

void SimpleCalendarCreator::saveWorker(const QString& path, const QString& createdTime)
{
    
    if (path.isEmpty()) return;

    QString modTime{ QDateTime::currentDateTimeUtc().toString(Qt::DateFormat::ISODate) };

    boost::property_tree::ptree meta;
    meta.add("spec.version", "1.0.0");
    meta.add("app.uid", SimpleCalendarCreator::app_uid.data());
    meta.add("app.version", SimpleCalendarCreator::app_version.data());
    meta.add("file.version", SimpleCalendarCreator::file_version.data());
    meta.add("file.modified", modTime.toStdString());

    if (createdTime.isEmpty())
        meta.add("file.created", modTime.toStdString());
    else
        meta.add("file.created", createdTime.toStdString());

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

    std::unique_ptr<libzip::archive> output;  //{ path.toStdString(), ZIP_CREATE };

    if (createdTime.isEmpty())
        output = std::make_unique<libzip::archive>(path.toStdString(), ZIP_CREATE);
    else
        output = std::make_unique<libzip::archive>(path.toStdString());

    auto writeOutput = [&output](const std::string & filename, const libzip::source & source) {
        try
        {
            auto info = output->stat(filename);
            output->replace(source, info.index);
        }
        catch (const std::runtime_error&)
        {
            output->add(source, filename);
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
    UndoHistory::getInstance()->changesSaved();
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

    setProjectName();
    auto tempCmd = std::make_unique<command::ResizeCalendar>(default_calender_size,
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

void SimpleCalendarCreator::onOpenProject()
{
    constexpr std::string_view error_dialog_title{ "Error on Opening File" };

    onNewProject();

    auto path = QFileDialog::getOpenFileName(this, "Open file...", {}, "Calendar design(*.calendar)");
    if (path.isEmpty()) return;

    std::unique_ptr<libzip::archive> container{ nullptr };
    try
    {
        container = std::make_unique<libzip::archive>(path.toStdString());
    }
    catch (const std::runtime_error & e)
    {
#ifdef _DEBUG
        qDebug() << e.what();
#endif // _DEBUG
        BOOST_ASSERT_MSG(false, "Critical error occured");
    }

    boost::property_tree::ptree metaIni;

    auto reader = [&container](const std::string & name) -> std::string {
        libzip::stat stat{ container->stat(name) };
        return container->open(stat.index).read(stat.size);
    };

    try
    {
        std::istringstream ss{ reader("_meta/meta.ini") };
        boost::property_tree::ini_parser::read_ini(ss, metaIni);
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(this, error_dialog_title.data(), e.what());
        return;
    }

    auto specVer = QString::fromStdString(metaIni.get<std::string>("spec.version"));
    auto appId = QString::fromStdString(metaIni.get<std::string>("app.uid"));
    auto fileVersion = QString::fromStdString(metaIni.get<std::string>("file.version"));
    if (appId != SimpleCalendarCreator::app_uid.data())
    {
        QMessageBox::critical(this, static_cast<std::string>(error_dialog_title).c_str(),
            "Simple Calendar Creator is unable to open this file.");
        return;
    }

    if (specVer > "1.0.0" || fileVersion > SimpleCalendarCreator::file_version.data())
    {
        QMessageBox::critical(this, error_dialog_title.data(),
            "Unable to open file, it's designed for newer program");
        return;
    }

    pugi::xml_document document;
    try
    {
        std::istringstream ss{ reader("design.xml") };
        auto result = document.load(ss);
        if (result.status != pugi::xml_parse_status::status_ok)
            throw std::runtime_error{ result.description() };
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(this, error_dialog_title.data(), e.what());
        return;
    }

    auto design = document.first_child();
    CalendarObjectFactory factory;
    for (auto itr : design)
    {
        using namespace std::string_literals;
        if (itr.name() != "calendar_obj"s) continue;
        try
        {
            auto item = new CustomListWidgetItem{ this, itr.attribute("name").as_string(),
                factory.createObject(itr.attribute("type").as_string()) };
            ui->objectList->addItem(item);
            item->getElement()->deserialize(itr);
        }
        catch (const std::out_of_range & e)
        {
#ifdef _DEBUG
            qDebug() << e.what();
#endif // _DEBUG
            continue;
        }
    }
    savedPath = path;
    setProjectName(savedPath.completeBaseName());
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

    auto appId = QString::fromStdString(metaIni.get<std::string>("app.uid"));
    if (appId != SimpleCalendarCreator::app_uid.data())
    {
        QMessageBox::critical(this, "Unsupported File Format",
            "This file is not created for Simple Calendar Creator");
        return;
    }

    auto verFile = QString::fromStdString(metaIni.get<std::string>("file.version"));
    auto verApp = QString::fromStdString(metaIni.get<std::string>("app.version"));
    if (verFile > SimpleCalendarCreator::file_version.data() ||
        verApp > SimpleCalendarCreator::app_version.data())
    {
        QMessageBox::critical(this, "Unsupported File Format",
            "This file is created for newer version of Simple Calendar Creator which is not supported.");
        return;
    }
    container = nullptr;
    saveWorker(savedPath.filePath(), QString::fromStdString(metaIni.get<std::string>("file.created")));
}

void SimpleCalendarCreator::onSaveProjectAs()
{
    QString path{ QFileDialog::getSaveFileName(this, "Save as", "", "Calendar design(*.calendar)") };
#ifdef _DEBUG
    qDebug() << "output path: " << path;
#endif // _DEBUG
    saveWorker(path);
}
