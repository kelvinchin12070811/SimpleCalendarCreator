/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/PreviewWindow.hpp"

#include <qaction.h>
#include <qpainter.h>

#include "element/CustomListWidgetItem.hpp"

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

void PreviewWindow::connectObjects()
{
    connect(ui->monthNext, &QPushButton::clicked, this, &PreviewWindow::onNextMonth);
    connect(ui->monthPrevios, &QPushButton::clicked, this, &PreviewWindow::onPrevMonth);
    connect(ui->monthToPreview, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &PreviewWindow::onPreviewMonthChanged);
}

void PreviewWindow::render(const QListWidget& list)
{
    QPainter painter;
    for (int idxMonth{ 0 }; idxMonth < 12; idxMonth++)
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
                    element_->render(static_cast<element::Month>(idxMonth)));
            }
        }
        painter.end();
        months.push_back(std::move(buffer));
    }
}

void PreviewWindow::initUi()
{
    QGraphicsScene* scene{ new QGraphicsScene };
    
    previewPixmap = new QGraphicsPixmapItem{ months[0] };
    scene->addItem(previewPixmap);

    ui->previewArea->setScene(scene);

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
