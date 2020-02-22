#ifndef MINDMAPWIDGETTOOLBAR_H
#define MINDMAPWIDGETTOOLBAR_H

#include "api/color/colortable.h"
#include "mindmapwidget.h"
#include <QColor>
#include <QPainter>
#include <QWidget>

namespace Ui {
class MindMapWidgetToolBar;
}
class MindMapWidget;
class MindMapWidgetToolBar : public QWidget {
    Q_OBJECT

public:
    explicit MindMapWidgetToolBar(QWidget* parent = nullptr);
    ~MindMapWidgetToolBar();
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void setMindMapWidget(MindMapWidget* mindMapWidget);
    void mousePressEvent(QMouseEvent* event);

private:
    Ui::MindMapWidgetToolBar* ui;
    QRect m_fontBoldRect;
    QRect m_fontItalicsRect;
    QRect m_underLineRect;
    QRect m_strikeOutRect;
    QRect m_overLineRect;
    QRect m_fontBackColorRect;
    QRect m_fontBackColorLeftRect;
    QRect m_fontBackColorRightRect;
    QRect m_fontColorRect;
    QRect m_fontColorLeftRect;
    QRect m_fontColorRightRect;
    QRect m_strewRect;
    QRect m_strewApplyRect;
    QPixmap m_dropdownPix;
    QPixmap m_pickerPix;
    QBrush getSelBrush(QPoint& pt, QRect& rc);
    QPen getSelPen(QPoint& pt, QRect& rc);
    QBrush getBkBrush();
    MindMapWidget* m_mindMapWidget;
};

#endif // MINDMAPWIDGETTOOLBAR_H
