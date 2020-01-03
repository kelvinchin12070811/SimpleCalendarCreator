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
#include <QDesktopServices>
#include <qevent.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qpainter.h>

#include "command/AddObject.hpp"
#include "command/RemoveObject.hpp"
#include "command/UndoHistory.hpp"
#include "element/CalendarObjectFactory.hpp"
#include "window/About.hpp"
#include "window/CalendarResizer.hpp"
#include "window/EditProjectInfo.hpp"
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
    return properties.szCalendar;
}

void SimpleCalendarCreator::setProjectName(const QString& value) noexcept
{
    projectName = value;
    this->setWindowTitle(QString{ SimpleCalendarCreator::window_title }.arg(value));
}

void SimpleCalendarCreator::closeEvent(QCloseEvent* ev)
{
    if (UndoHistory::getInstance()->hasUnsave())
    {
        auto rst = QMessageBox::warning(this, "Unsaved Work",
            "This operation will cause unsaved data lost, are you sure to continue?",
            QMessageBox::Yes | QMessageBox::No);
        if (rst != QMessageBox::Yes)
        {
            ev->ignore();
            return;
        }
    }

    ev->accept();
}

void SimpleCalendarCreator::resizeEvent(QResizeEvent* ev)
{
    ui->winOutline->fitInView(0, 0, static_cast<qreal>(properties.szCalendar.width()),
        static_cast<qreal>(properties.szCalendar.height()), Qt::AspectRatioMode::KeepAspectRatio);
    ui->winOutline->centerOn(properties.szCalendar.width() / 2.0, properties.szCalendar.height() / 2.0);
}

void SimpleCalendarCreator::connectObjects()
{
    connect(ui->actionAbout_Qt, &QAction::triggered,
        [this]() { QMessageBox::aboutQt(this, this->windowTitle()); });
    connect(ui->actionNew, &QAction::triggered, [this]() { onNewProject(); });
    connect(ui->actionOpen, &QAction::triggered, this, &SimpleCalendarCreator::onOpenProject);
    connect(ui->actionQuit, &QAction::triggered, [this]() { this->close(); });
    connect(ui->actionSave, &QAction::triggered, this, &SimpleCalendarCreator::onSaveProject);
    connect(ui->actionSave_As, &QAction::triggered, this, &SimpleCalendarCreator::onSaveProjectAs);
    connect(ui->actionUndo, &QAction::triggered, []() { UndoHistory::getInstance()->pop(); });
    connect(ui->btnAddObject, &QPushButton::clicked, this, &SimpleCalendarCreator::onAddObject);
    connect(ui->btnEditObject, &QPushButton::clicked, [this]() {
        auto itm = this->ui->objectList->currentItem();
        if (itm == nullptr) return;
        static_cast<CustomListWidgetItem*>(itm)->getElement()->edit(this);
    });
    connect(ui->btnEditProps, &QPushButton::clicked, [this]() {
        auto dialogue = std::make_unique<EditProjectInfo>(&properties,
            std::bind(&SimpleCalendarCreator::onPropertiesChanged, this), this);
        dialogue->exec();
    });
    connect(ui->btnRemoveObject, &QPushButton::clicked, this, &SimpleCalendarCreator::onRemoveObject);
    connect(ui->btnPreview, &QPushButton::clicked, [this]() {
        auto previewWindow = std::make_unique<PreviewWindow>(*ui->objectList, properties.selectedYear,
            properties.szCalendar, this);
        previewWindow->exec();
    });
}

void SimpleCalendarCreator::initUi()
{
    onNewProject();
}

void SimpleCalendarCreator::saveWorker(const QString& path, const QString& createdTime)
{
    
    if (path.isEmpty()) return;

    QString modTime{ QDateTime::currentDateTimeUtc().toString(Qt::DateFormat::ISODate) };

    boost::property_tree::ptree meta;
    meta.add("spec.version", "1.0.0");
    meta.add("app.uid", SimpleCalendarCreator::app_uid);
    meta.add("app.version", SimpleCalendarCreator::app_version);
    meta.add("file.version", SimpleCalendarCreator::file_version);
    meta.add("file.modified", modTime.toStdString());

    if (createdTime.isEmpty())
        meta.add("file.created", modTime.toStdString());
    else
        meta.add("file.created", createdTime.toStdString());

    pugi::xml_document document;
    auto declaration = document.append_child(pugi::xml_node_type::node_declaration);
    declaration.append_attribute("version").set_value("1.0");
    declaration.append_attribute("encoding").set_value("utf-8");

    auto design = document.append_child("design");
    auto project = design.append_child("project");
    project.append_child("target-year").text().set(properties.selectedYear);
    auto projectSize = project.append_child("size");
    projectSize.append_attribute("w").set_value(properties.szCalendar.width());
    projectSize.append_attribute("h").set_value(properties.szCalendar.height());

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

void SimpleCalendarCreator::onAbout()
{
    auto about = std::make_unique<About>(this);
    about->setWindowTitle(QString{ "About Simple Calendar Creator v" } +SimpleCalendarCreator::app_version);
    about->exec();
}

void SimpleCalendarCreator::onAddObject()
{
    UndoHistory::getInstance()->push(std::make_unique<command::AddObject>(this, ui->objectList));
}

void SimpleCalendarCreator::onGenerateCalendar()
{
    QString path{ QFileDialog::getExistingDirectory(this, "Render Calenders To...") };
    if (path.isEmpty()) return;

    QPixmap graphic{ properties.szCalendar };
    QPainter painter;
    QLocale locale{ QLocale::Language::English, QLocale::Country::UnitedKingdom };
    QString title{ this->windowTitle() };
    for (QDate date{ properties.selectedYear, 1, 1 }; date.year() == properties.selectedYear;
        date = date.addMonths(1))
    {
        this->setWindowTitle(QString{ "Generating item %1/12..." }.arg(date.month()));
        graphic.fill(Qt::GlobalColor::transparent);
        painter.begin(&graphic);
        painter.setRenderHint(QPainter::RenderHint::Antialiasing);
        for (int idx2{ 0 }; idx2 < ui->objectList->count(); idx2++)
        {
            auto item = static_cast<CustomListWidgetItem*>(ui->objectList->item(idx2));
            painter.drawPixmap(QRect{ QPoint{ 0, 0 }, properties.szCalendar },
                item->getElement()->render(date));
        }
        painter.end();
        graphic.save(QString{ "%1/%2 %3.png" }.arg(path).arg(QString::number(date.month()))
            .arg(locale.toString(date, "MMMM")), "PNG");
    }
    this->setWindowTitle(title);
}

bool SimpleCalendarCreator::onNewProject()
{
    if (UndoHistory::getInstance()->hasUnsave())
    {
        auto result = QMessageBox::information(this, "Unsaved work", QString{ "Operation will cause" }
            + " unsaved work lost, are your sure to continue?", QMessageBox::Yes | QMessageBox::No);

        if (result == QMessageBox::No) return false;
    }

    setProjectName();
    QDate today{ QDate::currentDate() };
    properties = {
        today.year(),
        SimpleCalendarCreator::default_calender_size
    };
    ui->labYear->setText(QString{ EditProjectInfo::format_targeted_year }.arg(properties.selectedYear));
    ui->szCalendarIndicator->setText(QString{ EditProjectInfo::format_calendar_size }
        .arg(properties.szCalendar.width()).arg(properties.szCalendar.height()));

    ui->objectList->clear();
    
    QGraphicsScene* scene{ ui->winOutline->scene() };
    if (scene != nullptr)  //Delete previous scene if exits
    {
        ui->winOutline->setScene(nullptr);
        delete scene;
    }
    scene = new QGraphicsScene;

    scene->setSceneRect(0, 0, static_cast<qreal>(properties.szCalendar.width()),
        static_cast<qreal>(properties.szCalendar.height()));
    ui->winOutline->setScene(scene);

    QPixmap border{ properties.szCalendar };
    border.fill(Qt::GlobalColor::white);

    QGraphicsPixmapItem* borderItem = new QGraphicsPixmapItem{ border };
    scene->addItem(borderItem);

    if (!this->isHidden())
    {
        this->resize(this->size().width() + 1, this->size().height() + 1);
        this->resize(this->size().width() - 1, this->size().height() - 1);
    }

    UndoHistory::getInstance()->changesSaved();
    return true;
}

void SimpleCalendarCreator::onOpenProject()
{
    constexpr std::string_view error_dialog_title{ "Error on Opening File" };

    auto path = QFileDialog::getOpenFileName(this, "Open file...", QDir::homePath(),
        "Calendar design(*.calendar)");
    if (path.isEmpty()) return;
    
    if (!onNewProject()) return;

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
    if (appId != SimpleCalendarCreator::app_uid)
    {
        QMessageBox::critical(this, static_cast<std::string>(error_dialog_title).c_str(),
            "Simple Calendar Creator is unable to open this file.");
        return;
    }

    if (specVer > "1.0.0" || fileVersion > SimpleCalendarCreator::file_version)
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
    auto project = design.child("project");
    properties.selectedYear = project.child("target-year").text().as_int(1997);
    
    auto projectSize = project.child("size");
    properties.szCalendar = QSize{
        projectSize.attribute("w").as_int(),
        projectSize.attribute("h").as_int()
    };
    onPropertiesChanged();

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

void SimpleCalendarCreator::onPropertiesChanged()
{
    ui->labYear->setText(QString{ EditProjectInfo::format_targeted_year }.arg(properties.selectedYear));
    ui->szCalendarIndicator->setText(QString{ EditProjectInfo::format_calendar_size }
        .arg(properties.szCalendar.width()).arg(properties.szCalendar.height()));

    for (int idx{ 0 }; idx < ui->objectList->count(); idx++)
    {
        auto item = dynamic_cast<CustomListWidgetItem*>(ui->objectList->item(idx));
        item->getElement()->setSize(properties.szCalendar);
    }
}

void SimpleCalendarCreator::onRemoveObject()
{
    auto undoHistory = UndoHistory::getInstance();
    undoHistory->push(std::make_unique<command::RemoveObject>(ui->objectList, ui->winOutline));
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
    if (appId != SimpleCalendarCreator::app_uid)
    {
        QMessageBox::critical(this, "Unsupported File Format",
            "This file is not created for Simple Calendar Creator");
        return;
    }

    auto verFile = QString::fromStdString(metaIni.get<std::string>("file.version"));
    auto verApp = QString::fromStdString(metaIni.get<std::string>("app.version"));
    if (verFile > SimpleCalendarCreator::file_version ||
        verApp > SimpleCalendarCreator::app_version)
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
    QString path{ QFileDialog::getSaveFileName(this, "Save as", QDir::homePath(),
        "Calendar design(*.calendar)") };
#ifdef _DEBUG
    qDebug() << "output path: " << path;
#endif // _DEBUG
    saveWorker(path);
}

void SimpleCalendarCreator::onShowDependencies()
{
    QUrl path{ "file:///" + QCoreApplication::applicationDirPath() + "/3rd party notice.zip" };
    bool result{ QDesktopServices::openUrl(path)};
    if (!result)
    {
        QMessageBox::information(this, "Operation Failed", "Failed to open " + path.toString());
    }
}
