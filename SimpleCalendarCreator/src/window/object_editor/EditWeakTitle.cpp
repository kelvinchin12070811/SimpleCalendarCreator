/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "EditWeakTitle.hpp"

#include <boost/assert.hpp>

#include <QColorDialog>
#include <QFontDialog>

#include "command/ChangeObjectProperties.hpp"
#include "command/UndoHistory.hpp"

EditWeakTitle::EditWeakTitle(element::object_properties::WeakTitle* properties, QWidget *parent)
    : QDialog(parent), properties(properties), ui(std::make_unique<Ui_EditWeakTitle>())
{
    BOOST_ASSERT_MSG(this->properties != nullptr, "properties must not be nullptr");
    ui->setupUi(this);
    initUi();
}

void EditWeakTitle::forwardConnect(std::function<void()> slot)
{
    BOOST_ASSERT_MSG(slot != nullptr, "slot must not be nullptr");
    propertiesChangedSlot = std::move(slot);
}

void EditWeakTitle::applyColourPreview(const QColor& colour, QLineEdit* hex, QLabel* visual)
{
    BOOST_ASSERT_MSG(hex != nullptr, "hex must not be nullptr");
    BOOST_ASSERT_MSG(visual != nullptr, "visual must not be nullptr");
    QString name{ colour.name(QColor::NameFormat::HexArgb) };
    hex->setText(name);
    visual->setStyleSheet("background-color: " + name);
}

void EditWeakTitle::applyFontPreview(const QFont& font, QLineEdit* fontName)
{
    BOOST_ASSERT_MSG(fontName != nullptr, "fontName must not be nullptr");
    fontName->setText(font.toString());
}

void EditWeakTitle::initUi()
{
    ui->isVertical->setChecked(properties->isVertical);
    ui->textAlignment->setCurrentIndex(properties->textAlignment - 1);

    weakDayColour = properties->normalTextColour;
    applyColourPreview(weakDayColour, ui->colWeakdayHex, ui->colWeak);

    satColour = properties->satTextColour;
    applyColourPreview(satColour, ui->colSatHex, ui->colSat);

    sunColour = properties->sunTextColour;
    applyColourPreview(sunColour, ui->colSunHex, ui->colSun);

    selectedFont = properties->font;
    applyFontPreview(selectedFont, ui->fontPreview);

    ui->posX->setValue(properties->fontRect.x());
    ui->posY->setValue(properties->fontRect.y());
    ui->width->setValue(properties->fontRect.width());
    ui->height->setValue(properties->fontRect.height());

    for (const auto& itr : properties->lables)
    {
        auto item = new QTreeWidgetItem;
        item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
        item->setText(0, itr.first);
        for (const auto& itr2 : itr.second)
        {
            auto child = new QTreeWidgetItem;
            child->setFlags(child->flags() | Qt::ItemFlag::ItemIsEditable);
            child->setText(0, itr2);
            item->addChild(child);
        }
        ui->labels->addTopLevelItem(item);
    }
}

void EditWeakTitle::onAccepted()
{
    element::object_properties::WeakTitle newProperties{
        ui->isVertical->isChecked(),
        static_cast<uint8_t>(ui->textAlignment->currentIndex() + 1),
        weakDayColour,
        satColour,
        sunColour,
        selectedFont,
        QRect{
            ui->posX->value(),
            ui->posY->value(),
            ui->width->value(),
            ui->height->value()
        },
        {}
    };

    newProperties.lables.reserve(ui->labels->topLevelItemCount());
    for (int idx{ 0 }; idx < ui->labels->topLevelItemCount(); idx++)
    {
        auto item = ui->labels->topLevelItem(idx);
        std::vector<QString> labels;
        labels.reserve(item->childCount());
        for (int idx2{ 0 }; idx2 < item->childCount(); idx2++)
            labels.push_back(item->child(idx2)->text(0));

        newProperties.lables.emplace_back(item->text(0), std::move(labels));
    }
    auto cmd = std::make_unique<command::ChangeObjectProperties<decltype(newProperties)>>(properties,
        newProperties);
    cmd->propertiesChanged.connect(propertiesChangedSlot);
    UndoHistory::getInstance()->push(std::move(cmd));
    this->close();
}

void EditWeakTitle::onAddItem()
{
    if (ui->labels->topLevelItemCount() >= 12) return;

    auto item = new QTreeWidgetItem;
    item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
    item->setText(0, "Unnamed");
    for (int x{ 0 }; x < 7; x++)
    {
        auto child = new QTreeWidgetItem;
        child->setText(0, "<null>");
        child->setFlags(child->flags() | Qt::ItemFlag::ItemIsEditable);
        item->addChild(child);
    }
    ui->labels->addTopLevelItem(item);
    if (ui->labels->topLevelItemCount() >= 12) ui->addItem->setEnabled(false);
}

void EditWeakTitle::onRemoveItem()
{
    auto selectedItem = ui->labels->currentItem();
    auto itemParent = selectedItem->parent();
    if (itemParent != nullptr)
        return;
    else
        ui->labels->takeTopLevelItem(ui->labels->indexOfTopLevelItem(selectedItem));

    delete selectedItem;

    if (ui->labels->topLevelItemCount() < 12 && !ui->addItem->isEnabled())
        ui->addItem->setEnabled(true);
}

void EditWeakTitle::onSelectWeakdayColour()
{
    auto colour = QColorDialog::getColor(weakDayColour, this, "Select Text Colour for Weakdays",
        QColorDialog::ColorDialogOption::ShowAlphaChannel);
    if (!colour.isValid()) return;

    weakDayColour = std::move(colour);
    applyColourPreview(weakDayColour, ui->colWeakdayHex, ui->colWeak);
}

void EditWeakTitle::onSelectWeakendColour()
{
    auto colour = QColorDialog::getColor(satColour, this, "Select Text Colour for Weakend",
        QColorDialog::ColorDialogOption::ShowAlphaChannel);
    if (!colour.isValid()) return;

    satColour = std::move(colour);
    applyColourPreview(satColour, ui->colWeakdayHex, ui->colWeak);
}

void EditWeakTitle::onSelectWeakstartColour()
{
    auto colour = QColorDialog::getColor(sunColour, this, "Select Text Colour for Weakstart",
        QColorDialog::ColorDialogOption::ShowAlphaChannel);
    if (!colour.isValid()) return;

    sunColour = std::move(colour);
    applyColourPreview(sunColour, ui->colWeakdayHex, ui->colWeak);
}

void EditWeakTitle::onSelectFont()
{
    bool ok{ false };
    auto font = QFontDialog::getFont(&ok, selectedFont, this, "Select Font for Labels");
    if (!ok) return;

    selectedFont = std::move(font);
    applyFontPreview(selectedFont, ui->fontPreview);
}
