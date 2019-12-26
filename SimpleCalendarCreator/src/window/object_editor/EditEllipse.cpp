/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/object_editor/EditEllipse.hpp"

#include <numeric>

#include <boost/assert.hpp>

#include <QColorDialog>

#include "command/ChangeObjectProperties.hpp"
#include "command/UndoHistory.hpp"

EditEllipse::EditEllipse(element::object_properties::Ellipse* properties, QWidget *parent)
    : QDialog(parent), properties(properties), ui(std::make_unique<Ui::EditEllipse>())
{
    ui->setupUi(this);
    initUi();
    connectObjects();
}

void EditEllipse::forwardConnect(std::function<void()> slot)
{
    BOOST_ASSERT_MSG(slot != nullptr, "slot must not be nullptr");
    propertiesChangedSlot = std::move(slot);
}

void EditEllipse::connectObjects()
{
    connect(ui->btnCancel, &QPushButton::clicked, this, &EditEllipse::close);
    connect(ui->btnOk, &QPushButton::clicked, this, &EditEllipse::onAccepted);
    connect(ui->selectBackgroundColour, &QPushButton::clicked, this, &EditEllipse::onSelectBackgroundColour);
    connect(ui->selectForegroundColour, &QPushButton::clicked, this, &EditEllipse::onSelectForegroundColour);
}

void EditEllipse::initUi()
{
    constexpr int max{ std::numeric_limits<int>::max() };
    ui->originX->setMaximum(max);
    ui->originX->setValue(properties->originPos.x());
    
    ui->originY->setMaximum(max);
    ui->originY->setValue(properties->originPos.y());

    ui->radX->setMaximum(max);
    ui->radX->setValue(properties->radiusX);
    
    ui->radY->setMaximum(max);
    ui->radY->setValue(properties->radiusY);

    ui->borderWidth->setMaximum(max);
    ui->borderWidth->setValue(properties->width);

    QString colourName{ properties->backgroundColour.name(QColor::NameFormat::HexArgb) };
    ui->clHexBackground->setText(colourName);
    ui->colBackgroundPreview->setStyleSheet("background-color: " + colourName);

    colourName = properties->foregroundColour.name(QColor::NameFormat::HexArgb);
    ui->clHexForeground->setText(colourName);
    ui->colForegroundPreview->setStyleSheet("background-color: " + colourName);
}

void EditEllipse::onAccepted()
{
    element::object_properties::Ellipse newProperties{
        ui->radX->value(),
        ui->radY->value(),
        ui->borderWidth->value(),
        QPoint{
            ui->originX->value(),
            ui->originY->value()
        },
        QColor{ ui->clHexForeground->text() },
        QColor{ ui->clHexBackground->text() }
    };
    auto cmd = std::make_unique<command::ChangeObjectProperties<element::object_properties::Ellipse>>(
        properties, newProperties);
    cmd->propertiesChanged.connect(propertiesChangedSlot);
    UndoHistory::getInstance()->push(std::move(cmd));
    this->close();
}

void EditEllipse::onSelectBackgroundColour()
{
    QColor selected{ QColorDialog::getColor(properties->backgroundColour, this, "Select Background Colour",
        QColorDialog::ColorDialogOption::ShowAlphaChannel) };

    if (selected == properties->backgroundColour) return;
    QString name{ selected.name(QColor::NameFormat::HexArgb) };
    ui->clHexBackground->setText(name);
    ui->colBackgroundPreview->setStyleSheet("background-color: " + name);
}

void EditEllipse::onSelectForegroundColour()
{
    QColor selected{ QColorDialog::getColor(properties->foregroundColour, this, "Select Foreground Colour",
        QColorDialog::ColorDialogOption::ShowAlphaChannel) };

    if (selected == properties->foregroundColour) return;
    QString name{ selected.name(QColor::NameFormat::HexArgb) };
    ui->clHexForeground->setText(name);
    ui->colForegroundPreview->setStyleSheet("background-color: " + name);
}
