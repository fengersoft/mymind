#include "pixmapviewer.h"
#include "ui_pixmapviewer.h"

PixmapViewer::PixmapViewer(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PixmapViewer)
{
    ui->setupUi(this);
}

PixmapViewer::~PixmapViewer()
{
    delete ui;
}

void PixmapViewer::paintEvent(QPaintEvent* event)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(rect(), QColor(97, 97, 97));
    if (!m_tempPix.isNull()) {
        QRect rc;
        rc.setRect(m_startPoint.x(), m_startPoint.y(), m_tempPix.width(), m_tempPix.height());
        painter.drawPixmap(rc, m_tempPix, m_tempPix.rect());
    }
    painter.end();
}

void PixmapViewer::setImage(QPixmap& image)
{
    m_image = image;
    m_startPoint = QPoint(1, 1);
    setFactor(1);
    m_mousedownFlag = false;
}

void PixmapViewer::setFactor(double factor)
{
    m_factor = factor;
    m_tempPix = m_image.scaled(m_image.width() * m_factor, m_image.height() * factor,
        Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    update();
}

void PixmapViewer::wheelEvent(QWheelEvent* event)
{
    if (event->delta() > 0) {
        setFactor(m_factor * 1.1);
    } else {
        setFactor(m_factor * 0.9);
    }
}

void PixmapViewer::mousePressEvent(QMouseEvent* event)
{
    m_mousedownFlag = true;
    m_mousedownPoint = mapFromGlobal(cursor().pos());
    update();
}

void PixmapViewer::mouseReleaseEvent(QMouseEvent* event)
{
    m_mousedownFlag = false;
    update();
}

void PixmapViewer::mouseMoveEvent(QMouseEvent* event)
{
    if (m_mousedownFlag) {
        QPoint pt = mapFromGlobal(cursor().pos());
        int x = pt.x() - m_mousedownPoint.x();
        int y = pt.y() - m_mousedownPoint.y();
        m_startPoint.setX(m_startPoint.x() + x);
        m_startPoint.setY(m_startPoint.y() + y);
        m_mousedownPoint = pt;
        update();
    }
}
