/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/object_editor/EditLine.hpp"

#include <numeric>

#include <qcolordialog.h>

#include "command/ChangeObjectProperties.hpp"
#include "command/UndoHistory.hpp"

EditLine::EditLine(element::object_properties::Line* properties, QWidget* parent)
    : QDialog(parent), ui(std::make_unique<Ui::EditLine>()), properties(properties)
{
    assert(properties != nullptr);
    ui->setupUi(this);
    connectObjects();
    initUi();
}

bool EditLine::isAccepted() const noexcept
{
    return accepted;
}

void EditLine::forwardConnect(std::function<void()> slot)
{
    signalReceiver = std::move(slot);
}

void EditLine::connectObjects()
{
    connect(ui->btnCancel, &QPushButton::clicked, this, &EditLine::close);
    connect(ui->btnChooseColour, &QPushButton::clicked, this, &EditLine::onSelectColour);
    connect(ui->btnOk, &QPushButton::clicked, this, &EditLine::onAccepted);
}

void EditLine::initUi()
{
    const int max{ std::numeric_limits<int>::max() };
    const int min{ std::numeric_limits<int>::min() };
    
    ui->spinX1->setMaximum(max);
    ui->spinX1->setMinimum(min);
    ui->spinX1->setValue(properties->posLineStart.x());

    ui->spinY1->setMaximum(max);
    ui->spinY1->setMinimum(min);
    ui->spinY1->setValue(properties->posLineStart.y());

    ui->spinX2->setMaximum(max);
    ui->spinX2->setMinimum(min);
    ui->spinX2->setValue(properties->posLineEnd.x());

    ui->spinY2->setMaximum(max);
    ui->spinY2->setMinimum(min);
    ui->spinY2->setValue(properties->posLineEnd.y());

    ui->spinWidth->setMaximum(max);
    ui->spinWidth->setValue(properties->lineWidth);

    ui->lnedColourHex->setText(properties->lineColour.name());
    ui->labColourPreview->setStyleSheet("background-color: " + properties->lineColour.name());
}

void EditLine::onAccepted()
{
    element::object_properties::Line newValues;
    newValues.lineColour = QColor{ ui->lnedColourHex->text() };
    newValues.lineWidth = ui->spinWidth->value();
    newValues.posLineEnd = QPoint{ ui->spinX2->value(), ui->spinY2->value() };
    newValues.posLineStart = QPoint{ ui->spinX1->value(), ui->spinY1->value() };

    auto cmd = std::make_unique<command::ChangeObjectProperties<element::object_properties::Line>>(
        properties,
        newValues
    );
    cmd->propertiesChanged.connect(signalReceiver);
    accepted = true;
    UndoHistory::getInstance()->push(std::move(cmd));
    this->close();
}

void EditLine::onSelectColour()
{
    QColor colour{ QColorDialog::getColor(properties->lineColour) };
    ui->lnedColourHex->setText(colour.name());
    ui->labColourPreview->setStyleSheet("background: " + colour.name());
}