#include "colorwidget.h"
#include "ui_colorwidget.h"

ColorWidget::ColorWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ColorWidget)
{
    ui->setupUi(this);
}

ColorWidget::~ColorWidget()
{
    delete ui;
}

void ColorWidget::setColor(QColor color)
{
    m_color = color;
    update();
}

QColor& ColorWidget::color()
{
    return m_color;
}

void ColorWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter;
    painter.begin(this);
    QRect rc;
    rc.setRect(1, 1, width() - 2, height() - 2);
    QPen pen;
    QColor c = hasFocus() == true ? QColor(117, 117, 117) : QColor(223, 223, 223);
    pen.setColor(c);
    painter.setPen(pen);

    painter.drawRect(rc);
    rc.setRect(2, 2, width() - 4, height() - 4);
    painter.fillRect(rc, QBrush(m_color));
    painter.end();
}

void ColorWidget::focusInEvent(QFocusEvent* event)
{
    update();
}

void ColorWidget::focusOutEvent(QFocusEvent* event)
{
    update();
}

void ColorWidget::mousePressEvent(QMouseEvent* event)
{
    setFocus();
    emit onClick(m_index, m_color);
    update();
}

void ColorWidget::setIndex(int index)
{
    m_index = index;
}
