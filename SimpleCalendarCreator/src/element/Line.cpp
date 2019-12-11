/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#include "element/CustomListWidgetItem.hpp"

#include <functional>
#include <map>
#include <numeric>

#include <qcolordialog.h>
#include <qdialog.h>
#include <qfile.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <quiloader.h>

#include "element/Line.hpp"

#ifdef _DEBUG
#include <qdebug.h>
#endif // _DEBUG

namespace element
{
    void Line::setParent(CustomListWidgetItem* parent)
    {
        this->parent = parent;
    }

    const QPixmap& Line::getRenderedGraphics()
    {
        return graphic;
    }
    
    void Line::edit()
    {
        QFile ui{ ":/resource/ui/edit_line.ui" };
        ui.open(QIODevice::ReadOnly);
        assert(ui.isOpen());
        QUiLoader loader;
        QDialog* dialog{ static_cast<QDialog*>(loader.load(&ui)) };
        dialog->setWindowTitle(dialog->windowTitle().arg(parent->text()));
        
        std::map<QSpinBox*, std::function<void(QSpinBox*)>> spinBoxes = {
            { dialog->findChild<QSpinBox*>("spinX1"), [this](QSpinBox* spinbox) {
                    spinbox->setValue(lineNodes[0].x());
                }
            },
            { dialog->findChild<QSpinBox*>("spinY1"), [this](QSpinBox* spinbox) {
                    spinbox->setValue(lineNodes[0].y());
                }
            },
            { dialog->findChild<QSpinBox*>("spinX2"), [this](QSpinBox* spinbox) {
                    spinbox->setValue(lineNodes[1].x());
                }
            },
            { dialog->findChild<QSpinBox*>("spinY2"), [this](QSpinBox* spinbox) {
                    spinbox->setValue(lineNodes[1].y());
                }
            },
            { dialog->findChild<QSpinBox*>("spinWidth"), [this](QSpinBox* spinbox) {
                    spinbox->setValue(lineWidth);
                }
            }
        };
        for (auto itr : spinBoxes)
        {
            assert(itr.first != nullptr);
            itr.first->setMaximum(std::numeric_limits<int>::max());
            itr.first->setMinimum(std::numeric_limits<int>::min());
            itr.second(itr.first);
        }

        dialog->connect(dialog->findChild<QPushButton*>("btnChooseColour"), &QPushButton::clicked, [=]() {
            auto colour = QColorDialog::getColor(lineColour, dialog);
#ifdef _DEBUG
            qDebug() << lineColour;
#endif // _DEBUG
        });
        dialog->connect(dialog->findChild<QPushButton*>("btnOk"), &QPushButton::clicked, [this, dialog]() {
            onAccepted(dialog);
        });
        dialog->connect(dialog->findChild<QPushButton*>("btnCancel"), &QPushButton::clicked,
            dialog, &QDialog::close);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    }
    
    void Line::drawLine()
    {
        QPixmap newGraphic{
            lineNodes[1].x() - lineNodes[0].x(),
            lineNodes[1].y() - lineNodes[0].y()
        };
        newGraphic.fill(Qt::GlobalColor::transparent);
        QPainter painter{ &newGraphic };
        QPen pen = painter.pen();
        pen.setColor(lineColour);
        painter.setPen(pen);
        painter.drawLine(lineNodes[0], lineNodes[1]);
        graphic = std::move(newGraphic);
        parent->renderOutline();
    }

    void Line::onAccepted(QDialog* dialog)
    {
        auto lnedColourHex = dialog->findChild<QLineEdit*>("lnedColourHex");
        auto spinWidth = dialog->findChild<QSpinBox*>("spinWidth");
        auto spinX1 = dialog->findChild<QSpinBox*>("spinX1");
        auto spinY1 = dialog->findChild<QSpinBox*>("spinY1");
        auto spinX2 = dialog->findChild<QSpinBox*>("spinX2");
        auto spinY2 = dialog->findChild<QSpinBox*>("spinY2");

        assert(lnedColourHex != nullptr);
        lineColour = QColor{ lnedColourHex->text() };
        lineWidth = spinWidth->value();
        lineNodes[0].setX(spinX1->value());
        lineNodes[0].setY(spinY1->value());
        lineNodes[1].setX(spinX2->value());
        lineNodes[1].setY(spinY2->value());
        drawLine();
        dialog->close();
    }
}