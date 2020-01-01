/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/object_editor/EditDates.hpp"

#include <QColorDialog>
#include <QFontDialog>

#include "command/ChangeObjectProperties.hpp"
#include "command/UndoHistory.hpp"

EditDates::EditDates(element::object_properties::Dates* properties, QWidget *parent)
    : QDialog(parent), properties(properties), ui(std::make_unique<Ui::EditDates>())
{
    BOOST_ASSERT_MSG(this->properties != nullptr, "properties must not be nullptr");
    ui->setupUi(this);
    initUi();
}

EditDates::~EditDates()
{
    ui = nullptr;
}

void EditDates::forwardConnect(std::function<void()> slot)
{
    BOOST_ASSERT_MSG(slot != nullptr, "slot must not be nullptr");
    propertiesChangedSlot = std::move(slot);
}

void EditDates::applyColourPreview(const QColor& colour, QLineEdit* hexPreview, QLabel* colPreview)
{
    QString name{ colour.name(QColor::NameFormat::HexArgb) };
    hexPreview->setText(name);
    colPreview->setStyleSheet("background-color: " + name);
}

void EditDates::applyFontPreview(const QFont& font, QLineEdit* fontPreview)
{
    fontPreview->setText(font.toString());
}

void EditDates::initUi()
{
    ui->textAlign->setCurrentIndex(properties->textAlign - 1);
    
    weakdayColour = properties->weakdayColour;
    applyColourPreview(weakdayColour, ui->colWeakdayHex, ui->colWeakdayPreview);

    weakendColour = properties->weakendColour;
    applyColourPreview(weakendColour, ui->colWeakendHex, ui->colWeakendPreview);

    weakstartColour = properties->weakstartColour;
    applyColourPreview(weakstartColour, ui->colWeakstartHex, ui->colWeakstartPreview);

    labelFont = properties->font;
    applyFontPreview(labelFont, ui->fontPreview);

    ui->posX->setValue(properties->drawArea.x());
    ui->posY->setValue(properties->drawArea.y());
    ui->width->setValue(properties->drawArea.width());
    ui->height->setValue(properties->drawArea.height());

    for (const auto& itr : properties->speacialDays)
    {
        using namespace element::object_properties;
        auto markersGroup = new QTreeWidgetItem;
        markersGroup->setFlags(markersGroup->flags() | Qt::ItemFlag::ItemIsEditable);
        markersGroup->setText(0, std::get<Dates::SpeacialDaysIndex::group_name>(itr));
        markersGroup->setText(1, std::get<Dates::SpeacialDaysIndex::group_colour>(itr));
        for (const auto& itr2 : std::get<Dates::SpeacialDaysIndex::group_members>(itr))
        {
            auto dateEvent = new QTreeWidgetItem;
            dateEvent->setFlags(dateEvent->flags() | Qt::ItemFlag::ItemIsEditable);
            dateEvent->setText(0, itr2.first);
            dateEvent->setText(1, itr2.second);
            markersGroup->addChild(dateEvent);
        }
        ui->markers->addTopLevelItem(markersGroup);
    }
}

void EditDates::onAccepted()
{
    element::object_properties::Dates newProperties{
        static_cast<uint8_t>(ui->textAlign->currentIndex() + 1),
        weakdayColour,
        weakendColour,
        weakstartColour,
        labelFont,
        QRect{
            ui->posX->value(),
            ui->posY->value(),
            ui->width->value(),
            ui->height->value()
        },
        {}
    };

    newProperties.speacialDays.reserve(ui->markers->topLevelItemCount());

    for (int idx{ 0 }; idx < ui->markers->topLevelItemCount(); idx++)
    {
        using namespace element::object_properties;
        auto markersGroup = ui->markers->topLevelItem(idx);
        decltype(newProperties.speacialDays)::value_type type;
        std::get<Dates::SpeacialDaysIndex::group_name>(type) = markersGroup->text(0);
        std::get<Dates::SpeacialDaysIndex::group_colour>(type) = markersGroup->text(1);
        
        auto& vector = std::get<Dates::SpeacialDaysIndex::group_members>(type);
        vector.reserve(markersGroup->childCount());
        for (int idx2{ 0 }; idx2 < markersGroup->childCount(); idx2++)
        {
            auto item = markersGroup->child(idx2);
            vector.emplace_back(item->text(0), item->text(1));
        }
        newProperties.speacialDays.push_back(std::move(type));
    }

    auto cmd = std::make_unique<command::ChangeObjectProperties<decltype(newProperties)>>(properties,
        newProperties);
    cmd->propertiesChanged.connect(propertiesChangedSlot);
    UndoHistory::getInstance()->push(std::move(cmd));

    this->close();
}

void EditDates::onAddItem()
{
    auto item = new QTreeWidgetItem;
    item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
    item->setText(0, "<group name>");
    item->setText(1, "<colour hex:#argb>");
    ui->markers->addTopLevelItem(item);
    ui->markers->editItem(item);
}

void EditDates::onAddChild()
{
    auto selected = ui->markers->currentItem();
    if (selected == nullptr) return;
    if (selected->parent() != nullptr) return;

    auto item = new QTreeWidgetItem;
    item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
    item->setText(0, "<event name>");
    item->setText(1, "<date:mm-dd>");
    selected->addChild(item);
    ui->markers->editItem(item);
}

void EditDates::onRemoveItem()
{
    std::unique_ptr<QTreeWidgetItem> selected{ ui->markers->currentItem() };
    if (selected == nullptr) return;

    if (auto parent = selected->parent(); parent != nullptr)
        parent->takeChild(parent->indexOfChild(selected.get()));
    else
        ui->markers->takeTopLevelItem(ui->markers->indexOfTopLevelItem(selected.get()));
}

void EditDates::onSelectWeakdayColour()
{
    auto colour = QColorDialog::getColor(weakdayColour, this, "Select Text Colour for Weakday labels",
        QColorDialog::ColorDialogOption::ShowAlphaChannel);
    if (!colour.isValid()) return;

    weakdayColour = std::move(colour);
    applyColourPreview(weakdayColour, ui->colWeakdayHex, ui->colWeakdayPreview);
}

void EditDates::onSelectWeakendColour()
{
    auto colour = QColorDialog::getColor(weakendColour, this, "Select Text Colour for Weakend labels",
        QColorDialog::ColorDialogOption::ShowAlphaChannel);
    if (!colour.isValid()) return;

    weakendColour = std::move(colour);
    applyColourPreview(weakendColour, ui->colWeakendHex, ui->colWeakendPreview);
}

void EditDates::onSelectWeakstartColour()
{
    auto colour = QColorDialog::getColor(weakstartColour, this, "Select Text Colour for Weakstart labels",
        QColorDialog::ColorDialogOption::ShowAlphaChannel);
    if (!colour.isValid()) return;

    weakstartColour = std::move(colour);
    applyColourPreview(weakstartColour, ui->colWeakstartHex, ui->colWeakstartPreview);
}

void EditDates::onSelectFont()
{
    bool ok{ false };
    auto font = QFontDialog::getFont(&ok, this->labelFont, this, "Select Font for Labels");
    if (!ok) return;

    labelFont = std::move(font);
    applyFontPreview(labelFont, ui->fontPreview);
}
