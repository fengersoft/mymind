#include "mindmapwidgettoolbar.h"
#include "ui_mindmapwidgettoolbar.h"

MindMapWidgetToolBar::MindMapWidgetToolBar(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MindMapWidgetToolBar)
{
    ui->setupUi(this);
    setMouseTracking(true);
    m_fontBoldRect.setRect(4, 4, 24, 24);
    m_fontItalicsRect.setRect(m_fontBoldRect.right() + 4, 4, 24, 24);
    m_underLineRect.setRect(m_fontItalicsRect.right() + 4, 4, 24, 24);
    m_strikeOutRect.setRect(m_underLineRect.right() + 4, 4, 24, 24);
    m_overLineRect.setRect(m_strikeOutRect.right() + 4, 4, 24, 24);
    m_fontBackColorRect.setRect(m_overLineRect.right() + 4, 4, 36, 24);
    m_fontBackColorLeftRect.setRect(m_overLineRect.right() + 4, 4, 24, 24);
    m_fontBackColorRightRect.setRect(m_fontBackColorLeftRect.right() + 1, 4, 12, 24);
    m_fontColorRect.setRect(m_fontBackColorRect.right() + 4, 4, 36, 24);
    m_fontColorLeftRect.setRect(m_fontBackColorRect.right() + 4, 4, 24, 24);
    m_fontColorRightRect.setRect(m_fontColorLeftRect.right() + 1, 4, 12, 24);

    m_dropdownPix = QPixmap(":/res/toolbar/dropdown.png");
}

MindMapWidgetToolBar::~MindMapWidgetToolBar()
{
    delete ui;
}

void MindMapWidgetToolBar::paintEvent(QPaintEvent* event)
{
    QPoint pt = mapFromGlobal(cursor().pos());
    QFont font;
    QPen pen;
    QColor color;
    QPainter painter;
    int colorIndex;
    painter.begin(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.fillRect(rect(), QBrush(QColor(252, 252, 252)));
    painter.setPen(QColor(240, 240, 240));
    painter.drawLine(0, height(), width(), height());

    //绘制加粗按钮
    painter.save();
    painter.setBrush(getSelBrush(pt, m_fontBoldRect));
    painter.setPen(Qt::NoPen);
    painter.drawRect(m_fontBoldRect);
    painter.setPen(Qt::black);
    font = painter.font();
    font.setPointSize(17);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(m_fontBoldRect, Qt::AlignCenter, "B");
    painter.restore();
    //绘制文字倾斜按钮
    painter.save();
    painter.setBrush(getSelBrush(pt, m_fontItalicsRect));
    painter.setPen(Qt::NoPen);
    painter.drawRect(m_fontItalicsRect);
    painter.setPen(Qt::black);
    font = painter.font();
    font.setPointSize(17);
    font.setItalic(true);
    painter.setFont(font);
    painter.drawText(m_fontItalicsRect, Qt::AlignCenter, "I");
    painter.restore();
    //绘制下划线按钮
    painter.save();
    painter.setBrush(getSelBrush(pt, m_underLineRect));
    painter.setPen(Qt::NoPen);
    painter.drawRect(m_underLineRect);
    painter.setPen(Qt::black);
    font = painter.font();
    font.setPointSize(17);
    font.setUnderline(true);
    painter.setFont(font);
    painter.drawText(m_underLineRect, Qt::AlignCenter, "U");
    painter.restore();
    //删除线按钮
    painter.save();
    painter.setBrush(getSelBrush(pt, m_strikeOutRect));
    painter.setPen(Qt::NoPen);
    painter.drawRect(m_strikeOutRect);
    painter.setPen(Qt::black);
    font = painter.font();
    font.setPointSize(17);
    font.setStrikeOut(true);
    painter.setFont(font);
    painter.drawText(m_strikeOutRect, Qt::AlignCenter, "D");
    painter.restore();

    //删除线按钮
    painter.save();
    painter.setBrush(getSelBrush(pt, m_overLineRect));
    painter.setPen(Qt::NoPen);
    painter.drawRect(m_overLineRect);
    painter.setPen(Qt::black);
    font = painter.font();
    font.setPointSize(17);
    font.setOverline(true);
    painter.setFont(font);
    painter.drawText(m_overLineRect, Qt::AlignCenter, "A");
    painter.restore();

    //绘制文字背景按钮
    painter.save();
    painter.setBrush(Qt::NoBrush);
    painter.setPen(getSelPen(pt, m_fontBackColorRect));
    painter.drawRect(m_fontBackColorRect);
    painter.fillRect(m_fontBackColorLeftRect, getSelBrush(pt, m_fontBackColorLeftRect));
    painter.setPen(Qt::black);
    font = painter.font();
    font.setPointSize(17);
    painter.setFont(font);
    QRect fontBackRc = m_fontBackColorRect;
    fontBackRc.setWidth(24);
    painter.drawText(fontBackRc, Qt::AlignRight | Qt::AlignVCenter, "ab");
    pen = painter.pen();
    pen.setWidth(4);
    colorIndex = ColorTable::backColorIndex;
    if (colorIndex == ColorTable::lastIndex()) {
        pen.setStyle(Qt::NoPen);
    } else {
        pen.setColor(ColorTable::getColor(colorIndex));
    }

    painter.fillRect(m_fontBackColorRightRect, getSelBrush(pt, m_fontBackColorRightRect));
    painter.setPen(pen);
    painter.drawLine(fontBackRc.left() + 8, fontBackRc.bottom() - 2, fontBackRc.left() + 22, fontBackRc.bottom() - 2);
    QRect fontBackDownRc = m_fontBackColorRect;
    fontBackDownRc.setLeft(m_fontBackColorRect.right() - 10);
    fontBackDownRc.setTop(m_fontBackColorRect.top() + 4);
    fontBackDownRc.setWidth(8);
    fontBackDownRc.setHeight(16);

    painter.drawPixmap(fontBackDownRc, m_dropdownPix, m_dropdownPix.rect());
    painter.restore();

    //绘制文字颜色按钮
    painter.save();
    painter.setBrush(Qt::NoBrush);
    painter.setPen(getSelPen(pt, m_fontColorRect));
    painter.drawRect(m_fontColorRect);
    painter.fillRect(m_fontColorLeftRect, getSelBrush(pt, m_fontColorLeftRect));
    painter.setPen(Qt::black);
    font = painter.font();
    font.setPointSize(17);
    painter.setFont(font);
    QRect fontRc = m_fontColorRect;
    fontRc.setWidth(24);
    QRect textRc = fontRc;
    textRc.setLeft(textRc.left() + 4);
    painter.drawText(textRc, Qt::AlignCenter, "A");
    pen = painter.pen();
    pen.setWidth(4);
    colorIndex = ColorTable::fontColorIndex;
    if (colorIndex == 0) {
        pen.setColor(Qt::black);
    } else {
        pen.setColor(ColorTable::getColor(colorIndex));
    }

    painter.fillRect(m_fontColorRightRect, getSelBrush(pt, m_fontColorRightRect));
    painter.setPen(pen);
    painter.drawLine(fontRc.left() + 8, fontRc.bottom() - 2, fontRc.left() + 22, fontRc.bottom() - 2);
    QRect fontDownRc = m_fontColorRect;
    fontDownRc.setLeft(m_fontColorRect.right() - 10);
    fontDownRc.setTop(m_fontColorRect.top() + 4);
    fontDownRc.setWidth(8);
    fontDownRc.setHeight(16);

    painter.drawPixmap(fontDownRc, m_dropdownPix, m_dropdownPix.rect());
    painter.restore();

    painter.end();
}

void MindMapWidgetToolBar::mouseMoveEvent(QMouseEvent* event)
{
    update();
}

void MindMapWidgetToolBar::setMindMapWidget(MindMapWidget* mindMapWidget)
{
    m_mindMapWidget = mindMapWidget;
}

void MindMapWidgetToolBar::mousePressEvent(QMouseEvent* event)
{
    QPoint pt = mapFromGlobal(cursor().pos());
    if (m_fontBoldRect.contains(pt)) {
        m_mindMapWidget->setNodeFontStyle(SET_BOLD);
    } else if (m_fontItalicsRect.contains(pt)) {
        m_mindMapWidget->setNodeFontStyle(SET_ITALICS);
    } else if (m_underLineRect.contains(pt)) {
        m_mindMapWidget->setNodeFontStyle(SET_UNDERLINE);
    } else if (m_overLineRect.contains(pt)) {
        m_mindMapWidget->setNodeFontStyle(SET_OVERLINE);
    } else if (m_strikeOutRect.contains(pt)) {
        m_mindMapWidget->setNodeFontStyle(SET_STRIKEOUT);
    } else if (m_fontBackColorLeftRect.contains(pt)) {
        m_mindMapWidget->setNodeFontStyle(SET_FONT_BACKCOLOR);
    } else if (m_fontBackColorRightRect.contains(pt)) {
        m_mindMapWidget->showBackColorEditDialog();
    } else if (m_fontColorLeftRect.contains(pt)) {
        m_mindMapWidget->setNodeFontStyle(SET_FONT_COLOR);
    } else if (m_fontColorRightRect.contains(pt)) {
        m_mindMapWidget->showFontColorEditDialog();
    }
}

QBrush MindMapWidgetToolBar::getSelBrush(QPoint& pt, QRect& rc)
{
    if (rc.contains(pt)) {
        return QBrush(QColor(197, 197, 197));
    } else {
        return QBrush(Qt::NoBrush);
    }
}

QPen MindMapWidgetToolBar::getSelPen(QPoint& pt, QRect& rc)
{
    if (rc.contains(pt)) {
        return QPen(QColor(197, 197, 197));
    } else {
        return QPen(Qt::NoPen);
    }
}
