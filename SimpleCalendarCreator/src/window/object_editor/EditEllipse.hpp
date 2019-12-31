/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <functional>
#include <memory>

#include <QDialog>

#include "element/Ellipse.hpp"
#include "ui_EditEllipse.h"

/**
 * @brief Dialog that allow user to modify properties of element::Ellipse.
 */
class EditEllipse : public QDialog
{
    Q_OBJECT

public:
    /**
     * Create new Dialog for EditEllipse.
     * @param properties Properties of ellipse object to modify, must not be nullptr.
     * @param parent Parent of the dialog, nullptr for no parent. Default as nullptr.
     */
    EditEllipse(element::object_properties::Ellipse* properties, QWidget *parent = nullptr);

    /**
     * Forward slot from caller to dialog, must not be nullptr.
     */
    void forwardConnect(std::function<void()> slot);

private:
    /**
     * @internal
     * Connect components to slots.
     */
    void connectObjects();
    /**
     * @internal
     * Additional steps to initialize ui.
     */
    void initUi();
private slots:
    /**
     * @internal
     * Slot when user accepted the changed properties.
     */
    void onAccepted();
    /**
     * @internal
     * Slot when user tend to select background colour for ellipse.
     */
    void onSelectBackgroundColour();
    /**
     * @internal
     * Slot when user tend to select foreground colour for ellipse.
     */
    void onSelectForegroundColour();

private:
    /**
     * @internal
     * Properties of Ellipse object to modify.
     */
    element::object_properties::Ellipse* properties{ nullptr };
    /**
     * @internal
     * Ui components of EditEllipse.
     */
    std::unique_ptr<Ui::EditEllipse> ui{ nullptr };
    /**
     * @internal
     * Slot that called when properties changed.
     */
    std::function<void()> propertiesChangedSlot{ nullptr };
};
