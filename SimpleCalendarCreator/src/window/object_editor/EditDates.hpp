/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>

#include <QDialog>

#include "element/Dates.hpp"
#include "ui_EditDates.h"

class EditDates : public QDialog
{
    Q_OBJECT

public:
    /**
     * Construct new dialogue.
     * @param properties Properties of element::Dates to modify, must not be nullptr.
     * @param parent Parent of dialog, nullptr for no parent. Default: nullptr.
     */
    EditDates(element::object_properties::Dates* properties, QWidget *parent = Q_NULLPTR);
    ~EditDates();

    /**
     * Forward slot to call when properties changed to command::ChangeObjectProperties.
     * @param slot Slot to forward, must not be nullptr.
     */
    void forwardConnect(std::function<void()> slot);

private:
    /**
     * @internal
     * Apply colour preview to selected preview areas.
     * @param colour Colour to preview.
     * @param hexPreview Area to preview colour hex value.
     * @param colPreview Area to preview colour.
     */
    void applyColourPreview(const QColor& colour, QLineEdit* hexPreview, QLabel* colPreview);
    /**
     * @internal
     * Apply font preview to selected preview area.
     * @param font Font to preview.
     * @param fontPreview Area to preview font.
     */
    void applyFontPreview(const QFont& font, QLineEdit* fontPreview);
    /**
     * @internal
     * Additional steps to initialize UI.
     */
    void initUi();

private slots:
    /**
     * @internal
     * Slot called when ok button is clicked.
     */
    void onAccepted();
    /**
     * @internal
     * Slot called when tend to add top level item.
     */
    void onAddItem();
    /**
     * @internal
     * Slot called when tend to add child to top level item.
     */
    void onAddChild();
    /**
     * @internal
     * Slot called when tend to remove item.
     */
    void onRemoveItem();
    /**
     * @internal
     * Slot called when tend to select text colour for weakday labels.
     */
    void onSelectWeakdayColour();
    /**
     * @internal
     * Slot called when tend to select text colour for weakend labels.
     */
    void onSelectWeakendColour();
    /**
     * @internal
     * Slot called when tend to select text colour for weakstart labels.
     */
    void onSelectWeakstartColour();
    /**
     * @internal
     * Slot called when tend to select font for labels.
     */
    void onSelectFont();

private:
    /**
     * @internal
     * Properties of element::Dates to modify, must not be nullptr.
     */
    element::object_properties::Dates* properties{ nullptr };
    /**
     * @internal
     * UI component of dialog.
     */
    std::unique_ptr<Ui::EditDates> ui{ nullptr };
    /**
     * @internal
     * Slot to call when properties changed.
     */
    std::function<void()> propertiesChangedSlot{ nullptr };
    /**
     * @internal
     * Colour to draw weakday labels.
     */
    QColor weakdayColour;
    /**
     * @internal
     * Colour to draw weakend labels.
     */
    QColor weakendColour;
    /**
     * @internal
     * Colour to draw weakstart labels.
     */
    QColor weakstartColour;
    /**
     * @internal
     * Font to draw labels.
     */
    QFont labelFont;
};
