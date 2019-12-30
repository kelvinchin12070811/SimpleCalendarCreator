/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/EditProjectInfo.hpp"

#include <boost/assert.hpp>

#include "command/ChangeObjectProperties.hpp"
#include "command/UndoHistory.hpp"

EditProjectInfo::EditProjectInfo(CalendarProperties* properties, std::function<void()> slot, QWidget *parent)
    : QDialog(parent), properties(properties), ui(std::make_unique<Ui::EditProjectInfo>()),
    propertiesChangedSlot(std::move(slot))
{
    BOOST_ASSERT_MSG(this->properties != nullptr, "properties must not be nullptr");
    BOOST_ASSERT_MSG(this->propertiesChangedSlot != nullptr, "slot must not be nullptr");
    ui->setupUi(this);
    initUi();
    connectObjects();
}

void EditProjectInfo::connectObjects()
{
    connect(ui->cancel, &QPushButton::clicked, this, &EditProjectInfo::close);
    connect(ui->ok, &QPushButton::clicked, this, &EditProjectInfo::onAccepted);
}

void EditProjectInfo::initUi()
{
    ui->width->setValue(properties->szCalendar.width());
    ui->height->setValue(properties->szCalendar.height());
    ui->targetYear->setValue(properties->selectedYear);
}

void EditProjectInfo::onAccepted()
{
    CalendarProperties newProperties{
        ui->targetYear->value(),
        QSize{
            ui->width->value(),
            ui->height->value()
        }
    };
    auto cmd = std::make_unique<command::ChangeObjectProperties<CalendarProperties>>(properties,
        newProperties);
    cmd->propertiesChanged.connect(propertiesChangedSlot);
    UndoHistory::getInstance()->push(std::move(cmd));
    this->close();
}
