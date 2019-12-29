/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>

#include <QDialog>

#include "element/TemplatedText.hpp"
#include "ui_EditTemplatedText.h"

/**
 * @brief Dialog that allow user to edit properties of element::TemplatedText
 */
class EditTemplatedText : public QDialog
{
    Q_OBJECT

public:
    /**
     * Create new Dialog
     * @param properties Propeties of element::TemplatedText to modify, must not be nullptr.
     * @param parent Parent of dialog, nullptr for no parent. Default as nullptr.
     */
    EditTemplatedText(element::object_properties::TemplatedText* properties, QWidget *parent = nullptr);
    /**
     * Forward propertiesChanged slot to command::ChangeProperties.
     * @param slot Slot called when the signal fired, must not be nullptr.
     */
    void forwardConnect(std::function<void()> slot);

private:
    /**
     * @internal
     * Apply preview to selected colour on UI.
     * @param colour Colour to apply.
     * @param hexVal Hex value viewer to preview the colour.
     * @param colPreview Viewer to preview the colour.
     */
    void applyPreviewColour(const QColor& colour, QLineEdit* hexVal, QLabel* colPreview);
    /**
     * @internal
     * Apply preview to given font.
     * @param font Font to preview.
     * @param preview Viewer to preview font.
     */
    void applyPreviewFont(const QFont& font, QLineEdit* preview);
    /**
     * @internal
     * Connect ui components to appropriate slot.
     */
    void connectObjects();
    /**
     * @internal
     * Additional steps to setup UI.
     */
    void initUi();
    
private slots:
    /**
     * @internal
     * Slot called when user clicked the Ok button.
     */
    void onAccepted();
    /**
     * @internal
     * Add text to list.
     */
    void onAddText();
    /**
     * @internal
     * When edit selected text.
     */
    void onEditText();
    /**
     * @internal
     * Remove text from list.
     */
    void onRemoveText();
    /**
     * @internal
     * Select new colour for text.
     */
    void onSelectColour();
    /**
     * @internal
     * Select new font for text.
     */
    void onSelectFont();

private:
    /**
     * @internal
     * Properties of element::TemplatedText to modify, must not be nullptr.
     */
    element::object_properties::TemplatedText* properties{ nullptr };
    /**
     * @internal
     * Ui components of dialog.
     */
    std::unique_ptr<Ui::EditTemplatedText> ui{ nullptr };
    /**
     * @internal
     * Slot called when properties changed, must not be nullptr.
     */
    std::function<void()> propertiesChangedSlot{ nullptr };
    /**
     * @internal
     * Selected colour for text.
     */
    QColor selectedColour;
    /**
     * @internal
     * Selected font for text.
     */
    QFont selectedFont;
};
