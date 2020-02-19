#ifndef MINDMAPWIDGETTOOLBAR_H
#define MINDMAPWIDGETTOOLBAR_H

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

private:
    Ui::MindMapWidgetToolBar* ui;
};

#endif // MINDMAPWIDGETTOOLBAR_H
