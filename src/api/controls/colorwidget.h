#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QPainter>
#include <QWidget>

namespace Ui {
class ColorWidget;
}

class ColorWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColorWidget(QWidget* parent = nullptr);
    ~ColorWidget();
    void setColor(QColor color);
    QColor& color();
    void paintEvent(QPaintEvent* event);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void setIndex(int index);
signals:
    void onClick(int index, QColor color);

private:
    Ui::ColorWidget* ui;
    QColor m_color;
    int m_index;
};

#endif // COLORWIDGET_H
