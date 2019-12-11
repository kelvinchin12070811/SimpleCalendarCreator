/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "CalendarResizer.hpp"

#include <numeric>

CalendarResizer::CalendarResizer(const QSize& curSize, QWidget *parent)
    : QDialog(parent), ui(std::make_unique<Ui::CalendarResizer>())
{
    ui->setupUi(this);
    connectObjects();
    initUi(curSize);
}

bool CalendarResizer::isAccepted() const noexcept
{
    return accepted;
}

QSize CalendarResizer::getDecidedSize() const noexcept
{
    return { ui->spnWidth->value(), ui->spnHeight->value() };
}

void CalendarResizer::connectObjects()
{
    connect(ui->btnCancel, &QPushButton::clicked, this, &CalendarResizer::close);
    connect(ui->btnOk, &QPushButton::clicked, this, &CalendarResizer::onAccepted);
}

void CalendarResizer::initUi(const QSize& curSize)
{
    constexpr int max = std::numeric_limits<int>::max();

    ui->spnHeight->setMaximum(max);
    ui->spnHeight->setValue(curSize.height());

    ui->spnWidth->setMaximum(max);
    ui->spnWidth->setValue(curSize.width());
}

void CalendarResizer::onAccepted()
{
    accepted = true;
    this->close();
}
