/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/object_editor/EditText.hpp"

#include <QColorDialog>
#include <QFontDialog>

#include "command/ChangeObjectProperties.hpp"
#include "command/UndoHistory.hpp"

EditText::EditText(element::object_properties::Text* properties, QWidget *parent)
    : QDialog(parent), properties(properties), ui(std::make_unique<Ui::EditText>())
{
    ui->setupUi(this);
    initUi();
    connectObjects();
}

void EditText::forwardConnect(std::function<void()> slot)
{
    propertiesChangedSlot = std::move(slot);
}

void EditText::connectObjects()
{
    connect(ui->btnCancel, &QPushButton::clicked, this, &EditText::close);
    connect(ui->btnOk, &QPushButton::clicked, this, &EditText::onAccepted);
    connect(ui->selectColour, &QPushButton::clicked, this, &EditText::onSelectColour);
    connect(ui->selectFont, &QPushButton::clicked, this, &EditText::onSelectFont);
}

void EditText::initUi()
{
    ui->text->setText(properties->text);
    ui->textAlignment->setCurrentIndex(properties->textAlignment - 1);
    ui->posX->setValue(properties->pos.x());
    ui->posY->setValue(properties->pos.y());
    ui->isVerticalAlignment->setChecked(properties->verticalText);
    ui->fontInfo->setText(properties->font.toString());
    selectedFont = properties->font;

    QString colName{ properties->textColour.name(QColor::NameFormat::HexArgb) };
    ui->colHex->setText(colName);
    ui->colPreview->setStyleSheet("background-color: " + colName);
}

void EditText::onAccepted()
{
    element::object_properties::Text newProperties{
        ui->isVerticalAlignment->isChecked(),
        static_cast<uint8_t>(ui->textAlignment->currentIndex() + 1),
        QColor{ ui->colHex->text() },
        selectedFont,
        QPoint{
            ui->posX->value(),
            ui->posY->value()
        },
        ui->text->text()
    };

    auto cmd = std::make_unique<command::ChangeObjectProperties<element::object_properties::Text>>(
        properties, newProperties);
    cmd->propertiesChanged.connect(propertiesChangedSlot);
    UndoHistory::getInstance()->push(std::move(cmd));
    this->close();
}

void EditText::onSelectColour()
{
    QColor selected{ QColorDialog::getColor(properties->textColour, this, "Select Text Colour",
        QColorDialog::ColorDialogOption::ShowAlphaChannel) };
    if (selected == properties->textColour) return;

    QString colName{ selected.name(QColor::NameFormat::HexArgb) };
    ui->colHex->setText(colName);
    ui->colPreview->setStyleSheet("background-color: " + colName);
}

void EditText::onSelectFont()
{
    bool ok{ false };
    QFont selected{ QFontDialog::getFont(&ok, selectedFont, this, "Select Font") };
    if (!ok) return;

    selectedFont = selected;
    ui->fontInfo->setText(selectedFont.toString());
}
