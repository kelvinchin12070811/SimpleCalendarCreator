/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>

#include <QDialog>

#include "element/WeakTitle.hpp"
#include "ui_EditWeakTitle.h"

/**
 * @brief Dialogue that allow user to modify properties of element::WeakTitle
 */
class EditWeakTitle : public QDialog
{
    Q_OBJECT

public:
    /**
     * Create new dialogue
     * @param properties Properties of element::WeakTitle to modify, must not be nullptr.
     * @param parent Parent of the dialogue, nullptr for no parent. Default as nullptr.
     */
    EditWeakTitle(element::object_properties::WeakTitle* properties, QWidget *parent = Q_NULLPTR);
    ~EditWeakTitle() noexcept = default;

    /**
     * Slot to forward to ChangeObjectProperties::propertiesChanged signal, must not be nullptr.
     */
    void forwardConnect(std::function<void()> slot);

private:
    /**
     * @internal
     * Apply colour preview
     * @param colour Colour to apply
     * @param hex Area to preview colour hex value, must not be nullptr.
     * @param visual Area to preview colour, must not be nullptr.
     */
    void applyColourPreview(const QColor& colour, QLineEdit* hex, QLabel* visual);
    /**
     * @internal
     * Apply font preview to selected area.
     * @param font Font to preview.
     * @param fontName Area to preview font.
     */
    void applyFontPreview(const QFont& font, QLineEdit* fontName);
    /**
     * @internal
     * Additional steps to initialize UI.
     */
    void initUi();

private slots:
    /**
     * @internal
     * Slot called when ok button clicked.
     */
    void onAccepted();
    /**
     * @internal
     * Slot called when tend to add item into lists.
     */
    void onAddItem();
    /**
     * @internal
     * Slot called when tend to remove selected item.
     */
    void onRemoveItem();
    /**
     * @internal
     * Slot called when tend to select colour for weakdays.
     */
    void onSelectWeakdayColour();
    /**
     * @internal
     * Slot called when tend to select colour for weakend.
     */
    void onSelectWeakendColour();
    /**
     * @internal
     * Slot called when tend to select colour for weakstart.
     */
    void onSelectWeakstartColour();
    /**
     * @internal
     * Slot called when tend to select font for label.
     */
    void onSelectFont();
private:
    /**
     * @internal
     * Properties of element::WeakTitle to modify, must not be nullptr.
     */
    element::object_properties::WeakTitle* properties{ nullptr };
    /**
     * @internal
     * UI components of dialogue.
     */
    std::unique_ptr<Ui_EditWeakTitle> ui{ nullptr };
    /**
     * @internal
     * Slot to forward to command::ChangeObjectProperties command.
     */
    std::function<void()> propertiesChangedSlot{ nullptr };
    /**
     * @internal
     * Colour to render weakday label.
     */
    QColor weakDayColour;
    /**
     * @internal
     * Colour to render saturday label.
     */
    QColor satColour;
    /**
     * @internal
     * Colour to render sunday label.
     */
    QColor sunColour;
    /**
     * @internal
     * Font to render the label.
     */
    QFont selectedFont;
};
