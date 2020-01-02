/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>

#include <QDialog>

#include "ui_About.h"

class About : public QDialog
{
    Q_OBJECT

public:
    About(QWidget *parent = Q_NULLPTR);
    ~About();

private:
    std::unique_ptr<Ui::About> ui{ nullptr };
};
