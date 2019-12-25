/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/object_editor/EditRectangle.hpp"

#include <numeric>

#include <boost/assert.hpp>

#include <QColorDialog>

#include "command/ChangeObjectProperties.hpp"
#include "command/UndoHistory.hpp"

EditRectangle::EditRectangle(element::object_properties::Rectangle* properties, QWidget *parent)
    : QDialog(parent), ui(std::make_unique<Ui::EditRectangle>()), properties(properties)
{
    BOOST_ASSERT_MSG(this->properties != nullptr, "properties must not be nullptr");
    ui->setupUi(this);

    connectObjects();
    initUi();
}

void EditRectangle::forwardConnect(std::function<void()> slot) noexcept
{
    BOOST_ASSERT_MSG(slot != nullptr, "slot must not be nullptr");
    propertiesChangedSlot = std::move(slot);
}

void EditRectangle::connectObjects()
{
    connect(ui->cancel, &QPushButton::clicked, this, &EditRectangle::close);
    connect(ui->ok, &QPushButton::clicked, this, &EditRectangle::onAccepted);
    connect(ui->selectBackground, &QPushButton::clicked, this, &EditRectangle::onSelectBackgroundColour);
    connect(ui->selectForeground, &QPushButton::clicked, this, &EditRectangle::onSelectForefroundColour);
}

void EditRectangle::initUi()
{
    constexpr int max{ std::numeric_limits<int>::max() };
    constexpr int min{ std::numeric_limits<int>::min() };

    ui->posX->setMinimum(min);
    ui->posX->setMaximum(max);
    ui->posX->setValue(properties->rect.x());

    ui->posY->setMinimum(min);
    ui->posY->setMaximum(max);
    ui->posY->setValue(properties->rect.y());

    ui->rectWidth->setMaximum(max);
    ui->rectWidth->setValue(properties->rect.width());

    ui->rectHeight->setMaximum(max);
    ui->rectHeight->setValue(properties->rect.height());

    ui->borderwidth->setMaximum(max);
    ui->borderwidth->setValue(properties->width);

    QString colourName{ properties->backgroundColour.name() };
    ui->clHexBackground->setText(colourName);
    ui->previewBackground->setStyleSheet("background-color: " + colourName);

    colourName = properties->foregroundColour.name();
    ui->clHexForeground->setText(colourName);
    ui->previewForeground->setStyleSheet("background-color: " + colourName);
}

void EditRectangle::onSelectBackgroundColour()
{
    auto colour = QColorDialog::getColor(properties->backgroundColour, this, "Select Background Colour");
    if (colour == properties->backgroundColour) return;

    auto colourName = colour.name();
    ui->clHexBackground->setText(colourName);
    ui->previewBackground->setStyleSheet("background-color: " + colourName);
}

void EditRectangle::onSelectForefroundColour()
{
    auto colour = QColorDialog::getColor(properties->foregroundColour, this, "Select Foreground Colour");
    if (colour == properties->foregroundColour) return;

    auto colourName = colour.name();
    ui->clHexForeground->setText(colourName);
    ui->previewForeground->setStyleSheet("background-color: " + colourName);
}

void EditRectangle::onAccepted()
{
    element::object_properties::Rectangle newValue{
        QRect{
            ui->posX->value(),
            ui->posY->value(),
            ui->rectWidth->value(),
            ui->rectHeight->value()
        },
        QColor{ ui->clHexForeground->text() },
        QColor{ ui->clHexBackground->text() },
        ui->borderwidth->value()
    };
    auto cmd = std::make_unique<command::ChangeObjectProperties<element::object_properties::Rectangle>>(
        properties, newValue);
    cmd->propertiesChanged.connect(propertiesChangedSlot);
    UndoHistory::getInstance()->push(std::move(cmd));
    this->close();
}
