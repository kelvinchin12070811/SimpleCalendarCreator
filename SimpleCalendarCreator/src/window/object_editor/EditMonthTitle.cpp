/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/object_editor/EditMonthTitle.hpp"

#include <algorithm>

#include <boost/assert.hpp>

#include <QColorDialog>
#include <QFontDialog>

#include "command/ChangeObjectProperties.hpp"
#include "command/UndoHistory.hpp"

#ifdef _DEBUG
#include <QDebug>
#endif // _DEBUG

EditMonthTitle::EditMonthTitle(element::object_properties::MonthTitle* properties, QWidget *parent)
    : QDialog(parent), properties(properties), ui(std::make_unique<Ui::EditMonthTitle>())
{
    BOOST_ASSERT_MSG(this->properties != nullptr, "properties must not be nullptr");
    ui->setupUi(this);
    initUi();
    connectObjects();
}

void EditMonthTitle::forwardConnect(std::function<void()> slot)
{
    BOOST_ASSERT_MSG(slot != nullptr, "slot must not be nullptr");
    propertiesChangedSlot = std::move(slot);
}

void EditMonthTitle::applyColourPreview(const QColor& colour, QLineEdit* prevHex, QLabel* prevCol)
{
    QString name{ colour.name(QColor::NameFormat::HexArgb) };
    prevHex->setText(name);
    prevCol->setStyleSheet("background-color: " + name);
}

void EditMonthTitle::applyFontPreview(const QFont& font, QLineEdit* previewArea)
{
    previewArea->setText(font.toString());
}

void EditMonthTitle::connectObjects()
{
    connect(ui->cancel, &QPushButton::clicked, this, &EditMonthTitle::close);
    connect(ui->ok, &QPushButton::clicked, this, &EditMonthTitle::onAccepted);
    connect(ui->selectColour, &QPushButton::clicked, this, &EditMonthTitle::onSelectColour);
    connect(ui->selectFont, &QPushButton::clicked, this, &EditMonthTitle::onSelectFont);
}

void EditMonthTitle::initUi()
{
    auto localeList = QLocale::matchingLocales(QLocale::Language::AnyLanguage, QLocale::Script::AnyScript,
        QLocale::Country::AnyCountry);
    for (auto& itr : localeList)
    {
        ui->selectLocale->addItem(QString{ "%1 - %2" }.arg(itr.nativeLanguageName())
            .arg(itr.nativeCountryName()));
    }

    ui->posX->setValue(properties->pos.x());
    ui->posY->setValue(properties->pos.y());

    selectedColour = properties->textColour;
    applyColourPreview(selectedColour, ui->colHex, ui->colPreview);

    selectedFont = properties->font;
    applyFontPreview(selectedFont, ui->fontPreview);

    QString selectedLocale{ "%1 - %2" };
    selectedLocale = selectedLocale.arg(properties->locale.nativeLanguageName(),
        properties->locale.nativeCountryName());
    ui->selectLocale->setCurrentText(selectedLocale);
    ui->isVertical->setChecked(properties->isVertical);
}

void EditMonthTitle::onAccepted()
{
    QString selectedLocaleName{ ui->selectLocale->currentText() };
    auto list = selectedLocaleName.split('-', QString::SplitBehavior::SkipEmptyParts);

    auto allLocale = QLocale::matchingLocales(QLocale::Language::AnyLanguage, QLocale::Script::AnyScript,
        QLocale::Country::AnyCountry);
    auto rst = std::find_if(allLocale.begin(), allLocale.end(),
        [&list](decltype(allLocale)::reference itr) {
            return itr.nativeLanguageName() == list[0] && itr.nativeCountryName() == list[1];
        });

#ifdef _DEBUG
    if (rst == allLocale.end())
    {
        qDebug() << "Unable to find locale: " << selectedLocaleName;
    }
#endif // _DEBUG

    element::object_properties::MonthTitle newProperties{
        ui->isVertical->isChecked(),
        rst == allLocale.end() ? properties->locale : *rst,
        QPoint{ ui->posX->value(), ui->posY->value() },
        selectedFont,
        selectedColour
    };

    auto cmd = std::make_unique<command::ChangeObjectProperties<decltype(newProperties)>>(
        properties, newProperties);
    cmd->propertiesChanged.connect(propertiesChangedSlot);
    UndoHistory::getInstance()->push(std::move(cmd));

    this->close();
}

void EditMonthTitle::onSelectColour()
{
    auto colour = QColorDialog::getColor(selectedColour, this, "Select Title Colour",
        QColorDialog::ColorDialogOption::ShowAlphaChannel);
    if (!colour.isValid()) return;

    applyColourPreview(colour, ui->colHex, ui->colPreview);
    selectedColour = std::move(colour);
}

void EditMonthTitle::onSelectFont()
{
    bool ok{ false };
    auto font = QFontDialog::getFont(&ok, selectedFont, this, "Select Font for Title");
    if (!ok) return;

    applyFontPreview(font, ui->fontPreview);
    selectedFont = std::move(font);
}
