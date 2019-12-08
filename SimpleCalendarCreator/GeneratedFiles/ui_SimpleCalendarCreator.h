/********************************************************************************
** Form generated from reading UI file 'SimpleCalendarCreator.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLECALENDARCREATOR_H
#define UI_SIMPLECALENDARCREATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SimpleCalendarCreatorClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SimpleCalendarCreatorClass)
    {
        if (SimpleCalendarCreatorClass->objectName().isEmpty())
            SimpleCalendarCreatorClass->setObjectName(QString::fromUtf8("SimpleCalendarCreatorClass"));
        SimpleCalendarCreatorClass->resize(600, 400);
        menuBar = new QMenuBar(SimpleCalendarCreatorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        SimpleCalendarCreatorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SimpleCalendarCreatorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        SimpleCalendarCreatorClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SimpleCalendarCreatorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        SimpleCalendarCreatorClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SimpleCalendarCreatorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        SimpleCalendarCreatorClass->setStatusBar(statusBar);

        retranslateUi(SimpleCalendarCreatorClass);

        QMetaObject::connectSlotsByName(SimpleCalendarCreatorClass);
    } // setupUi

    void retranslateUi(QMainWindow *SimpleCalendarCreatorClass)
    {
        SimpleCalendarCreatorClass->setWindowTitle(QCoreApplication::translate("SimpleCalendarCreatorClass", "SimpleCalendarCreator", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SimpleCalendarCreatorClass: public Ui_SimpleCalendarCreatorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLECALENDARCREATOR_H
