/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/PreviewWindow.hpp"

#include <QAction>
#include <QDate>
#include <qevent.h>
#include <QPainter>

#include "element/CustomListWidgetItem.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif // _DEBUG

PreviewWindow::PreviewWindow(const QListWidget& list, const QSize& size, QWidget *parent)
    : QDialog(parent), szCalendar(size),ui(std::make_unique<Ui::PreviewWindow>())
{
    ui->setupUi(this);
    render(list);
    connectObjects();
    initUi();
}

PreviewWindow::~PreviewWindow() noexcept
{
    if (previewPixmap != nullptr && previewPixmap->parentWidget() != nullptr)
        delete previewPixmap;
}

void PreviewWindow::setYear(int year)
{
    this->selectedYear = year;
}

void PreviewWindow::showEvent(QShowEvent* ev)
{
    onFitInView();
    ev->accept();
}

void PreviewWindow::connectObjects()
{
    connect(ui->fitInView, &QPushButton::clicked, this, &PreviewWindow::onFitInView);
    connect(ui->monthNext, &QPushButton::clicked, this, &PreviewWindow::onNextMonth);
    connect(ui->monthPrevios, &QPushButton::clicked, this, &PreviewWindow::onPrevMonth);
    connect(ui->monthToPreview, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &PreviewWindow::onPreviewMonthChanged);
    connect(ui->resetZoom, &QPushButton::clicked, this, &PreviewWindow::onResetZoom);
    connect(ui->zoomIn, &QPushButton::clicked, this, &PreviewWindow::onZoomIn);
    connect(ui->zoomLevel, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &PreviewWindow::onZoomLevelChanged);
    connect(ui->zoomOut, &QPushButton::clicked, this, &PreviewWindow::onZoomOut);
}

void PreviewWindow::render(const QListWidget& list)
{
    QPainter painter;
    for (QDate date{ selectedYear, 1, 1 }; date.month() <= 12; date = date.addMonths(1))
    {
        QPixmap buffer{ szCalendar };
        buffer.fill(Qt::GlobalColor::white);
        painter.begin(&buffer);

        //Render all calendar object on to buffer
        for (int idxItem{ 0 }; idxItem < list.count(); idxItem++)
        {
            CustomListWidgetItem* item = static_cast<CustomListWidgetItem*>(list.item(idxItem));
            element::Element* element_ = item->getElement();
            if (element_ != nullptr)
            {
                painter.drawPixmap(QRect{ QPoint{ 0, 0 }, szCalendar },
                    element_->render(date));
            }
        }
        painter.end();
        months.push_back(std::move(buffer));
    }
}

void PreviewWindow::initUi()
{
    ui->zoomLevel->setMaximum(std::numeric_limits<int>::max());
    ui->zoomLevel->setValue(100);

    QGraphicsScene* scene{ new QGraphicsScene };
    
    previewPixmap = new QGraphicsPixmapItem{ months[0] };
    scene->addItem(previewPixmap);

    ui->previewArea->setScene(scene);
}

void PreviewWindow::onFitInView()
{
    double ratio{
        (static_cast<double>(ui->previewArea->viewport()->width()) / static_cast<double>(szCalendar.width()))
        * 100.0
    };
    ui->previewArea->fitInView(previewPixmap, Qt::AspectRatioMode::KeepAspectRatio);
    ui->zoomLevel->blockSignals(true);
    ui->zoomLevel->setValue(static_cast<int>(ratio));
    ui->zoomLevel->blockSignals(false);
}

void PreviewWindow::onNextMonth()
{
    int curIndex{ ui->monthToPreview->currentIndex() };
    ui->monthToPreview->setCurrentIndex(++curIndex);
}

void PreviewWindow::onPrevMonth()
{
    int curIndex{ ui->monthToPreview->currentIndex() };
    ui->monthToPreview->setCurrentIndex(--curIndex);
}

void PreviewWindow::onPreviewMonthChanged(int value)
{
    QGraphicsScene* scene{ ui->previewArea->scene() };
    assert(scene != nullptr);

    if (previewPixmap != nullptr)
    {
        scene->removeItem(previewPixmap);
        delete previewPixmap;
    }
    previewPixmap = new QGraphicsPixmapItem{ months[value] };
    scene->addItem(previewPixmap);

    if (value >= ui->monthToPreview->count() - 1)
        ui->monthNext->setEnabled(false);
    else
        ui->monthNext->setEnabled(true);

    if (value <= 0)
        ui->monthPrevios->setEnabled(false);
    else
        ui->monthPrevios->setEnabled(true);
}

void PreviewWindow::onResetZoom()
{
    ui->previewArea->resetTransform();
    ui->zoomLevel->setValue(100);
}

void PreviewWindow::onZoomIn()
{
    int current{ ui->zoomLevel->value() };
    ui->zoomLevel->setValue(current + PreviewWindow::zoom_amount);
}

void PreviewWindow::onZoomLevelChanged(int value)
{
    ui->previewArea->resetTransform();
    ui->previewArea->scale(value / 100.0, value / 100.0);
}

void PreviewWindow::onZoomOut()
{
    int current{ ui->zoomLevel->value() };
    ui->zoomLevel->setValue(current - PreviewWindow::zoom_amount);
}
