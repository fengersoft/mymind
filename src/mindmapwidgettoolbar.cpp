#include "mindmapwidgettoolbar.h"
#include "ui_mindmapwidgettoolbar.h"

MindMapWidgetToolBar::MindMapWidgetToolBar(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MindMapWidgetToolBar)
{
    ui->setupUi(this);
}

MindMapWidgetToolBar::~MindMapWidgetToolBar()
{
    delete ui;
}

void MindMapWidgetToolBar::paintEvent(QPaintEvent* event)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(rect(), QBrush(QColor(252, 252, 252)));
    painter.setPen(QColor(240, 240, 240));
    painter.drawLine(0, height(), width(), height());
    painter.end();
}
