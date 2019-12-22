/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "window/ObjectCreator.hpp"

#include <boost/assert.hpp>

#include <qmetaobject.h>
#include <qstringlist.h>

#include "element/CalendarObjectFactory.hpp"

ObjectCreator::ObjectCreator(QWidget *parent)
    : QDialog(parent), ui(std::make_unique<Ui::ObjectCreator>())
{
    ui->setupUi(this);

    initUi();
    connectObjects();
}

bool ObjectCreator::isAccepted() const
{
    return accepted;
}

std::unique_ptr<element::Element> ObjectCreator::createElement() const
{
    if (!isAccepted()) return nullptr;
    try
    {
        CalendarObjectFactory factory;
        auto objClassName = factory.getObjectClassName(ui->cmbObjectType->currentText());
        return factory.createObject(objClassName);
    }
    catch (const std::out_of_range & e)
    {
        BOOST_ASSERT_MSG(false, "element not registered.");
        return nullptr;
    }
}

QString ObjectCreator::getObjectName() const noexcept
{
    return ui->lneObjectName->text();
}

void ObjectCreator::connectObjects()
{
    connect(ui->btnCancel, &QPushButton::clicked, this, &ObjectCreator::close);
    connect(ui->btnOk, &QPushButton::clicked, this, &ObjectCreator::onAccepted);
}

void ObjectCreator::initUi()
{
    CalendarObjectFactory factory;
    for (auto&& itr : factory.getObjectReadableName())
    {
        ui->cmbObjectType->addItem(itr);
    }
}

void ObjectCreator::onAccepted()
{
    accepted = true;
    this->close();
}
