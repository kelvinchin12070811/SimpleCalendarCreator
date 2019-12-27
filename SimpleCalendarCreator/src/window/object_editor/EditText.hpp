/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <functional>
#include <memory>

#include <QDialog>

#include "element/Text.hpp"
#include "ui_EditText.h"

/**
 * @brief Dialog that allow user to modify properties of the element::Text object.
 */
class EditText : public QDialog
{
    Q_OBJECT

public:
    /**
     * Create new dialog.
     * @param properties Properties of Text object to modify, must not be nullptr.
     * @param parent Parent of the dialog, nullptr for no parent.
     */
    EditText(element::object_properties::Text* properties, QWidget *parent = Q_NULLPTR);

    /**
     * Forward slot to command::ChangeObjectProperties.
     */
    void forwardConnect(std::function<void()> slot);

private:
    /**
     * @internal
     * Connect objects to slots.
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
     * Slot called when user click the Ok button.
     */
    void onAccepted();
    /**
     * @internal
     * Slot called when user tend to choose new colour for Text object.
     */
    void onSelectColour();
    /**
     * @internal
     * Slot called when user tend to choose a new font.
     */
    void onSelectFont();

private:
    /**
     * @internal
     * Properties of the element::Text object to modify, must not be nullptr.
     */
    element::object_properties::Text* properties{ nullptr };
    /**
     * @internal
     * Ui components of the dialog.
     */
    std::unique_ptr<Ui::EditText> ui{ nullptr };
    /**
     * @internal
     * Slot called when properties of Text object changed, must not be nullptr.
     */
    std::function<void()> propertiesChangedSlot{ nullptr };
    /**
     * @internal
     * Font that selected by the user.
     */
    QFont selectedFont;
};
