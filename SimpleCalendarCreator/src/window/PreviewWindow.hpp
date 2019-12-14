/************************************************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
************************************************************************************************************/
#pragma once
#include <memory>
#include <vector>

#include <QDialog>
#include <qgraphicsitem.h>
#include <qlistwidget.h>
#include <qpixmap.h>

#include "ui_PreviewWindow.h"

/**
 * @brief Window to preview the rendered calendar.
 */
class PreviewWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Construct new preview window.
     * @param list List of calendar objects to render months' graphics.
     * @param size Size of the calendar design.
     * @param parent Parent of this dialog.
     */
    PreviewWindow(const QListWidget& list, const QSize& size, QWidget *parent = Q_NULLPTR);
    ~PreviewWindow() noexcept;

private:
    /**
     * @internal
     * @brief Connect objects with slots.
     */
    void connectObjects();
    /**
     * @internal
     * @brief Extra instruction to setup ui.
     */
    void initUi();
    /**
     * @internal
     * @brief Render months with provided list.
     */
    void render(const QListWidget& list);
private slots:  //Slots
    /**
     * @internal
     * @brief Show the next month when requested by user.
     */
    void onNextMonth();
    /**
     * @internal
     * @brief Show the previous month when requested by user.
     */
    void onPrevMonth();
    /**
     * @internal
     * @brief On the index of combo box changed.
     */
    void onPreviewMonthChanged(int value);

private:  //Attributes
    /**
     * @internal
     * @brief Ui elements of PreviewWindow.
     */
    std::unique_ptr<Ui::PreviewWindow> ui{ nullptr };
    /**
     * @internal
     * @brief Rendered months image.
     */
    std::vector<QPixmap> months;
    
    /**
     * @internal
     * @brief Current pixmap to preview.
     */
    QGraphicsPixmapItem* previewPixmap{ nullptr };
    /**
     * @internal
     * @brief Size of the calendar design.
     */
    QSize szCalendar;
};
