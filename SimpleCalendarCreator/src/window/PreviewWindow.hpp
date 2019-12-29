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
    /** Amount of zoom increment or decrement. */
    static constexpr int zoom_amount{ 5 };
public:
    /**
     * Construct new preview window.
     * @param list List of calendar objects to render months' graphics.
     * @param size Size of the calendar design.
     * @param parent Parent of this dialog.
     */
    PreviewWindow(const QListWidget& list, const QSize& size, QWidget *parent = Q_NULLPTR);
    ~PreviewWindow() noexcept;

    /**
     * Set the target year to generate calendar.
     * @param year target year to generate calendar.
     */
    void setYear(int year);

protected:
    /**
     * @internal
     * Event handaler when the dialog is presented.
     */
    void showEvent(QShowEvent* ev) override;
private:
    /**
     * @internal
     * Connect objects with slots.
     */
    void connectObjects();
    /**
     * @internal
     * Extra instruction to setup ui.
     */
    void initUi();
    /**
     * @internal
     * Render months with provided list.
     */
    void render(const QListWidget& list);
private slots:  //Slots
    /**
     * @internal
     * Fit the graphics in preview window.
     */
    void onFitInView();
    /**
     * @internal
     * Show the next month when requested by user.
     */
    void onNextMonth();
    /**
     * @internal
     * Show the previous month when requested by user.
     */
    void onPrevMonth();
    /**
     * @internal
     * On the index of combo box changed.
     */
    void onPreviewMonthChanged(int value);
    /**
     * @internal
     * Slot when reseting zoom to 100%
     */
    void onResetZoom();
    /**
     * @internal
     * Slot when zooming view in.
     */
    void onZoomIn();
    /**
     * @internal
     * Slot when zoom level changed. Handaler to zoom in or out.
     */
    void onZoomLevelChanged(int value);
    /**
     * @internal
     * Slot when zooming view out.
     */
    void onZoomOut();
private:  //Attributes
    /**
     * @internal
     * Target year to generate with calendar.
     */
    int selectedYear{ 1997 };
    /**
     * @internal
     * Ui elements of PreviewWindow.
     */
    std::unique_ptr<Ui::PreviewWindow> ui{ nullptr };
    /**
     * @internal
     * Rendered months image.
     */
    std::vector<QPixmap> months;
    
    /**
     * @internal
     * Current pixmap to preview.
     */
    QGraphicsPixmapItem* previewPixmap{ nullptr };
    /**
     * @internal
     * Size of the calendar design.
     */
    QSize szCalendar;
};
