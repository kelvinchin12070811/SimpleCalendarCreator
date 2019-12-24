/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include "element/Element.hpp"

#include <functional>
#include <map>
#include <memory>

#include <qdialog.h>

#include "ui_ObjectCreator.h"

/**
 * @brief Dialog that ask user to create a new calendar element and add it into their calendar design.
 */
class ObjectCreator : public QDialog
{
    Q_OBJECT

public:
    ObjectCreator(QWidget *parent = Q_NULLPTR);
    ~ObjectCreator() = default;

    /** Determine if the input is accepted by the user (hit Ok button). */
    bool isAccepted() const;
    /**
     * Create the specific element determined by the user.
     * @return Instance of a calendar element or nullptr if user does not accept the input.
     * @warning Assertion occur if user selected element does not registered.
     */
    std::unique_ptr<element::Element> createElement() const;
    /**
     * Get the name of the object will be created.
     */
    QString getObjectName() const noexcept;

private:
    void connectObjects();
    void initUi();

private: // slots
    /**
     * @internal
     * Slot when "Ok" button is clicked.
     */
    void onAccepted();

private:
    /**
     * @internal
     * Determine if current session of create object dialog is accepted by user.
     */
    bool accepted{ false };
    /**
     * @internal
     * Ui of the Object Creator.
     */
    std::unique_ptr<Ui::ObjectCreator> ui{ nullptr };
};
