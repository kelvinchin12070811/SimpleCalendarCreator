/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <functional>
#include <memory>

#include <QDialog>

#include "element/Rectangle.hpp"
#include "ui_EditRectangle.h"

/**
 * @brief Edit dialog box for rectangle object.
 */
class EditRectangle : public QDialog
{
    Q_OBJECT

public:
    /**
     * Create new EditRectangle dialog.
     * @param properties Properties to modify, must not be nullptr.
     * @param parent Parent of the dialog, nullptr for no parent.
     */
    EditRectangle(element::object_properties::Rectangle* properties, QWidget *parent = nullptr);
    ~EditRectangle() noexcept = default;

    /**
     * Forward slot from element::Rectangle to EditRectangle, must not be empty.
     */
    void forwardConnect(std::function<void()> slot) noexcept;
private:
    /**
     * @internal
     * Connect objects with slots.
     */
    void connectObjects();
    /**
     * @internal
     * Initialize UI with additional steps.
     */
    void initUi();

private slots:
    /**
     * @internal
     * Slot when select colour button of background colour is clicked.
     */
    void onSelectBackgroundColour();
    /**
     * @internal
     * Slot when select colour button of foreground colour is clicked.
     */
    void onSelectForefroundColour();
    /**
     * @internal
     * Slot when user clicked the ok button.
     */
    void onAccepted();
private:
    /**
     * @internal
     * Properties object that targeted to modify.
     */
    element::object_properties::Rectangle* properties{ nullptr };
    /**
     * @internal
     * Ui components of the dialog.
     */
    std::unique_ptr<Ui::EditRectangle> ui;
    /**
     * @internal
     * Slot that called when the properties changed.
     */
    std::function<void()> propertiesChangedSlot{ nullptr };
};
