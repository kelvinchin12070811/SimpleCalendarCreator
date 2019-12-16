/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <functional>
#include <memory>

#include <QDialog>

#include "element/Line.hpp"
#include "ui_EditLine.h"

/**
 * @brief Dialog that allow user to modify properties of the calendar object element::Line.
 */
class EditLine : public QDialog
{
    Q_OBJECT

public:
    /**
     * Create new EditLine dialog.
     * @param properties Properties of line object. Can't be nullptr.
     * @param parent Parent of the dialog.
     */
    EditLine(element::object_properties::Line* properties, QWidget *parent = Q_NULLPTR);
    ~EditLine() noexcept = default;

    /**
     * @brief Determine if user has accepted the modefied properties.
     */
    bool isAccepted() const noexcept;
    
    /**
     * @brief Foward reciver to slots which will only connected when needed for
     * command::object_properties_modifier::LineObject.
     */
    void forwardConnect(std::function<void()> slot);

private:
    /**
     * @internal
     * @brief Connect objects to its appropraite slots.
     */
    void connectObjects();
    /**
     * @internal
     * @brief Extra instruction to setup ui.
     */
    void initUi();

private slots:
    /**
     * @internal
     * @brief Slots when user accepted the modified properties.
     */
    void onAccepted();
    /**
     * @internal
     * @brief Slots when user decided to change the line's colour.
     */
    void onSelectColour();

private:
    /**
     * @internal
     * @brief Determine if user has accepted the modified properties.
     */
    bool accepted{ false };

    /**
     * @internal
     * @brief Ui elements of EditLine dialog.
     */
    std::unique_ptr<Ui::EditLine> ui{ nullptr };
    /**
     * @internal
     * @brief Properties to modify.
     */
    element::object_properties::Line* properties{ nullptr };
    /**
     * @internal
     * @brief Receiver of the propertiesChanged signal of command::object_properties_modifier::LineObject.
     */
    std::function<void()> signalReceiver{ nullptr };
};
