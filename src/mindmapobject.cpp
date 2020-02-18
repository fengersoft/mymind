#include "mindmapobject.h"

MindMapObject::MindMapObject(QObject* parent)
    : QObject(parent)
{
    m_parentObj = nullptr;
    QColor colors[12] = { QColor(229, 77, 66), QColor(243, 123, 29), QColor(251, 189, 8),
        QColor(141, 198, 63), QColor(57, 181, 74), QColor(28, 187, 180), QColor(0, 129, 255),
        QColor(103, 57, 182), QColor(156, 38, 176), QColor(224, 57, 151), QColor(165, 103, 63), QColor(135, 153, 163) };
    int i = qrand() % 12;
    m_lineColor = colors[i];
    i = qrand() % 12;
    m_rootColor = colors[i];
    m_fontColor = 0;
    m_backColor = ColorTable::lastIndex();
}

void MindMapObject::setPid(int pid)
{
    m_pid = pid;
}

int MindMapObject::id()
{
    return m_id;
}

int MindMapObject::pid()
{
    return m_pid;
}

void MindMapObject::setId(int id)
{
    m_id = id;
}

void MindMapObject::setName(QString name)
{
    m_name = name;
}

QString MindMapObject::name()
{
    return m_name;
}

void MindMapObject::setParentObj(MindMapObject* parentObj)
{
    m_parentObj = parentObj;
}

MindMapObject* MindMapObject::parentObj()
{
    return m_parentObj;
}

QColor MindMapObject::lineColor()
{
    return m_lineColor;
}

QColor MindMapObject::rootColor()
{
    return m_rootColor;
}

void MindMapObject::setRemark(QString remark)
{
    m_remark = remark;
}

QString MindMapObject::remark()
{
    return m_remark;
}

void MindMapObject::setLink(QString link)
{
    m_link = link;
}

QString MindMapObject::link()
{
    return m_link;
}

void MindMapObject::setFontColor(int fontColor)
{
    fontColor == -1 ? 0 : fontColor;
    m_fontColor = fontColor;
}

int MindMapObject::fontColor()
{
    return m_fontColor;
}

void MindMapObject::setBackColor(int backColor)
{
    m_backColor = backColor;
}

int MindMapObject::backColor()
{
    return m_backColor;
}

MindMapObject::~MindMapObject()
{
}

void MindMapObject::setSxh(int sxh)
{
    m_sxh = sxh;
}

int MindMapObject::sxh()
{
    return m_sxh;
}
