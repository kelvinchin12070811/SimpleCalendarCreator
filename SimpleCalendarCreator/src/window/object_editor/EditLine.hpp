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
     * Determine if user has accepted the modefied properties.
     */
    bool isAccepted() const noexcept;
    
    /**
     * Foward reciver to slots which will only connected when needed for
     * command::object_properties_modifier::LineObject.
     */
    void forwardConnect(std::function<void()> slot);

private:
    /**
     * @internal
     * Connect objects to its appropraite slots.
     */
    void connectObjects();
    /**
     * @internal
     * Extra instruction to setup ui.
     */
    void initUi();

private slots:
    /**
     * @internal
     * Slots when user accepted the modified properties.
     */
    void onAccepted();
    /**
     * @internal
     * Slots when user decided to change the line's colour.
     */
    void onSelectColour();

private:
    /**
     * @internal
     * Determine if user has accepted the modified properties.
     */
    bool accepted{ false };

    /**
     * @internal
     * Ui elements of EditLine dialog.
     */
    std::unique_ptr<Ui::EditLine> ui{ nullptr };
    /**
     * @internal
     * Properties to modify.
     */
    element::object_properties::Line* properties{ nullptr };
    /**
     * @internal
     * Receiver of the propertiesChanged signal of command::object_properties_modifier::LineObject.
     */
    std::function<void()> signalReceiver{ nullptr };
};
