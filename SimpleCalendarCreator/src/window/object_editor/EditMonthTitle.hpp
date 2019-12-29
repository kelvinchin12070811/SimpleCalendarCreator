/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>

#include <QDialog>

#include "element/MonthTitle.hpp"
#include "ui_EditMonthTitle.h"

class EditMonthTitle : public QDialog
{
    Q_OBJECT

public:
    /**
     * Create new dialog.
     * @param properties Properties of element::MonthTitle object to modify, must not be nullptr.
     * @param parent Parent of the dialog, nullptr for no parent. Default be nullptr.
     */
    EditMonthTitle(element::object_properties::MonthTitle* properties, QWidget *parent = Q_NULLPTR);
    ~EditMonthTitle() noexcept = default;

    /**
     * Connect external slot to propertiesChanged signal.
     * @param slot Slot that will be forwarded to propertiesChanged signal, must not be nullptr.
     */
    void forwardConnect(std::function<void()> slot);

private:
    /**
     * @internal
     * Apply preview for selected colour.
     * @param prevHex Area to preview hex value of @p colour, must not be nullptr.
     * @param prevCol Area to preview colour of @p colour, must not be nullptr.
     */
    void applyColourPreview(const QColor& colour, QLineEdit* prevHex, QLabel* prevCol);
    /**
     * @internal
     * Apply preview for selected font.
     * @param font Font to preview.
     * @param previewArea Area to preview font, must not be nullptr.
     */
    void applyFontPreview(const QFont& font, QLineEdit* previewArea);
    /**
     * @internal
     * Connect each objects to its appropaite slot.
     */
    void connectObjects();
    /**
     * @internal
     * Additional steps to initialize UI.
     */
    void initUi();

private slots:
    /**
     * @internal
     * Slot called when user clicked on Ok button.
     */
    void onAccepted();
    /**
     * @internal
     * Slot called when user tend to select new colour.
     */
    void onSelectColour();
    /**
     * @internal
     * Slot called when user tend to selecet new font.
     */
    void onSelectFont();
private:
    /**
     * @internal
     * Properties of element::MonthTitle object to modify, must not be nullptr.
     */
    element::object_properties::MonthTitle* properties{ nullptr };
    /**
     * @internal
     * UI components of dialog.
     */
    std::unique_ptr<Ui::EditMonthTitle> ui{ nullptr };
    /**
     * @internal
     * Slot called when properties changed.
     */
    std::function<void()> propertiesChangedSlot{ nullptr };
    /**
     * @internal
     * Selected colour for title.
     */
    QColor selectedColour;
    /**
     * @internal
     * Selected font for title.
     */
    QFont selectedFont;
};
