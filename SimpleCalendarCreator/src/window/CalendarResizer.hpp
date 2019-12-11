/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>

#include <QDialog>

#include "ui_CalendarResizer.h"

/**
 * @brief Dialog that let user to resize their calendar design.
 */
class CalendarResizer : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Create resizer dialog with current calendar size.
     * @param curSize Current size of the calendar design.
     * @param parent Parent widget of the dialog, nullptr for no parent.
     */
    CalendarResizer(const QSize& curSize, QWidget *parent = Q_NULLPTR);
    ~CalendarResizer() noexcept = default;

    /**
     * @brief Determine if user has accepted the values.
     */
    bool isAccepted() const noexcept;
    /**
     * @brief Get new size selected by user.
     */
    QSize getDecidedSize() const noexcept;
private:
    void connectObjects();
    void initUi(const QSize& curSize);

private:  //slots
    /**
     * @internal
     * @breif Slots when user accept the new properties.
     */
    void onAccepted();

private:
    /**
     * @internal
     * @brief Determine if user accepted the new properties.
     */
    bool accepted{ false };
    /**
     * @internal
     * @brief Ui elements of CalendarResizer's dialog
     */
    std::unique_ptr<Ui::CalendarResizer> ui{ nullptr };
};
