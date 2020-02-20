#ifndef MINDMAPWIDGETTOOLBAR_H
#define MINDMAPWIDGETTOOLBAR_H

#include "api/color/colortable.h"
#include <QColor>
#include <QPainter>
#include <QWidget>
namespace Ui {
class MindMapWidgetToolBar;
}

class MindMapWidgetToolBar : public QWidget {
    Q_OBJECT

public:
    explicit MindMapWidgetToolBar(QWidget* parent = nullptr);
    ~MindMapWidgetToolBar();
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

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
    QPixmap m_dropdownPix;
    QBrush getSelBrush(QPoint& pt, QRect& rc);
    QPen getSelPen(QPoint& pt, QRect& rc);
};

#endif // MINDMAPWIDGETTOOLBAR_H
