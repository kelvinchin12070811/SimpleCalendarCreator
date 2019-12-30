/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/object_editor/EditTemplatedText.hpp"

#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>

#include "command/ChangeObjectProperties.hpp"
#include "command/UndoHistory.hpp"
#include "element/CalendarObjectFactory.hpp"

EditTemplatedText::EditTemplatedText(element::object_properties::TemplatedText* properties, QWidget *parent)
    : QDialog(parent), properties(properties), ui(std::make_unique<Ui::EditTemplatedText>())
{
    BOOST_ASSERT_MSG(this->properties != nullptr, "properties must not be nullptr");
    ui->setupUi(this);
    initUi();
    connectObjects();
}

void EditTemplatedText::forwardConnect(std::function<void()> slot)
{
    BOOST_ASSERT_MSG(slot != nullptr, "slot must not be nullptr");
    propertiesChangedSlot = std::move(slot);
}

void EditTemplatedText::applyPreviewColour(const QColor& colour, QLineEdit* hexVal, QLabel* colPreview)
{
    QString name{ colour.name(QColor::NameFormat::HexArgb) };
    hexVal->setText(name);
    colPreview->setStyleSheet("background-color: " + name);
}

void EditTemplatedText::applyPreviewFont(const QFont& font, QLineEdit* preview)
{
    preview->setText(font.toString());
}

void EditTemplatedText::connectObjects()
{
    connect(ui->addText, &QPushButton::clicked, this, &EditTemplatedText::onAddText);
    connect(ui->cancel, &QPushButton::clicked, this, &EditTemplatedText::close);
    connect(ui->editText, &QPushButton::clicked, this, &EditTemplatedText::onEditText);
    connect(ui->ok, &QPushButton::clicked, this, &EditTemplatedText::onAccepted);
    connect(ui->removeText, &QPushButton::clicked, this, &EditTemplatedText::onRemoveText);
    connect(ui->selectColour, &QPushButton::clicked, this, &EditTemplatedText::onSelectColour);
    connect(ui->selectFont, &QPushButton::clicked, this, &EditTemplatedText::onSelectFont);
}

void EditTemplatedText::initUi()
{
    ui->posX->setValue(properties->pos.x());
    ui->posY->setValue(properties->pos.y());

    selectedFont = properties->font;
    applyPreviewFont(selectedFont, ui->fontPreview);

    selectedColour = properties->textColour;
    applyPreviewColour(selectedColour, ui->colHex, ui->colPreview);

    ui->textAlign->setCurrentIndex(properties->textAlign - 1);
    ui->isVertical->setChecked(properties->isVertical);
    ui->texts->addItems(properties->texts);
}

void EditTemplatedText::onAccepted()
{
    QStringList texts;
    texts.reserve(ui->texts->count());
    for (int idx{ 0 }; idx < ui->texts->count(); idx++)
    {
        auto item = ui->texts->item(idx);
        texts.push_back(item->text());
    }

    element::object_properties::TemplatedText newProperties{
        ui->isVertical->isChecked(),
        static_cast<uint8_t>(ui->textAlign->currentIndex() + 1),
        selectedColour,
        selectedFont,
        QPoint{ ui->posX->value(), ui->posY->value() },
        std::move(texts)
    };
    auto cmd = std::make_unique<command::ChangeObjectProperties<decltype(newProperties)>>(properties,
        newProperties);
    cmd->propertiesChanged.connect(propertiesChangedSlot);
    UndoHistory::getInstance()->push(std::move(cmd));
    this->close();
}

void EditTemplatedText::onAddText()
{
    bool ok{ false };
    QString input{ QInputDialog::getText(this, "Add Text", "Text:", QLineEdit::EchoMode::Normal, QString{},
        &ok) };
    if (!ok) return;

    ui->texts->addItem(input);
    if (ui->texts->count() >= 12)
        ui->addText->setEnabled(false);
}

void EditTemplatedText::onEditText()
{
    bool ok{ false };
    auto item = ui->texts->currentItem();
    if (item == nullptr) return;

    QString text{ QInputDialog::getText(this, "Edit Text", "Text:", QLineEdit::EchoMode::Normal,
        item->text(), &ok) };
    if (!ok) return;
    item->setText(text);
}

void EditTemplatedText::onRemoveText()
{
    auto selected = ui->texts->currentIndex();
    if (selected.row() < 0) return;
    
    auto item = ui->texts->takeItem(selected.row());
    delete item;

    if (ui->texts->count() < 12)
        ui->addText->setEnabled(true);
}

void EditTemplatedText::onSelectColour()
{
    auto colour = QColorDialog::getColor(selectedColour, this, "Select Text's Colour",
        QColorDialog::ColorDialogOption::ShowAlphaChannel);
    if (!colour.isValid()) return;

    selectedColour = std::move(colour);
    applyPreviewColour(selectedColour, ui->colHex, ui->colPreview);
}

void EditTemplatedText::onSelectFont()
{
    bool ok{ false };
    auto font = QFontDialog::getFont(&ok, selectedFont, this, "Select Text's Font");
    if (!ok) return;

    selectedFont = std::move(font);
    applyPreviewFont(selectedFont, ui->fontPreview);
}
