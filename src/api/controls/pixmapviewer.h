#ifndef PIXMAPVIEWER_H
#define PIXMAPVIEWER_H

#include <QPainter>
#include <QPixmap>
#include <QWheelEvent>
#include <QWidget>

namespace Ui {
class PixmapViewer;
}

class PixmapViewer : public QWidget {
    Q_OBJECT

public:
    explicit PixmapViewer(QWidget* parent = nullptr);
    ~PixmapViewer();
    void paintEvent(QPaintEvent* event);
    void setImage(QPixmap& image);
    void setFactor(double factor);
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    Ui::PixmapViewer* ui;
    QPixmap m_image;
    QPixmap m_tempPix;
    double m_factor;
    QPoint m_mousedownPoint;
    QPoint m_startPoint;
    bool m_mousedownFlag;
};

#endif // PIXMAPVIEWER_H
