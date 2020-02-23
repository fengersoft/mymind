#include "mindmapobject.h"

MindMapObject::MindMapObject(QObject* parent)
    : QObject(parent)
{

    m_parentObj = nullptr;
    static QColor colors[12] = { QColor(229, 77, 66), QColor(243, 123, 29), QColor(251, 189, 8),
        QColor(141, 198, 63), QColor(57, 181, 74), QColor(28, 187, 180), QColor(0, 129, 255),
        QColor(103, 57, 182), QColor(156, 38, 176), QColor(224, 57, 151), QColor(165, 103, 63), QColor(135, 153, 163) };
    int i = qrand() % 12;
    m_lineColor = colors[i];
    i = qrand() % 12;
    m_rootColor = colors[i];
    m_fontColor = 0;
    m_backColor = ColorTable::lastIndex();
    m_showNum = false;
    m_newObj = nullptr;
    m_srcObj = nullptr;
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
    if (m_newObj != nullptr) {
        delete m_newObj;
    }
}

void MindMapObject::setSxh(int sxh)
{
    m_sxh = sxh;
}

int MindMapObject::sxh()
{
    return m_sxh;
}

void MindMapObject::setShowNum(int showNum)
{
    m_showNum = showNum;
}

bool MindMapObject::showNum()
{
    return m_showNum;
}

void MindMapObject::setBold(bool bold)
{
    m_bold = bold;
}

bool MindMapObject::bold()
{
    return m_bold;
}

void MindMapObject::setItalics(bool italics)
{
    m_italics = italics;
}

bool MindMapObject::italics()
{
    return m_italics;
}

void MindMapObject::setOverline(bool overline)
{
    m_overline = overline;
}

bool MindMapObject::overline()
{
    return m_overline;
}

void MindMapObject::setUnderline(bool underline)
{
    m_underline = underline;
}

bool MindMapObject::underline()
{
    return m_underline;
}

void MindMapObject::setStrikeOut(bool strikeOut)
{
    m_strikeOut = strikeOut;
}

bool MindMapObject::strikOut()
{
    return m_strikeOut;
}

void MindMapObject::setValue(int setType, bool value)
{
    if (setType == SET_BOLD) {
        setBold(value);
    } else if (setType == SET_ITALICS) {
        setItalics(value);
    } else if (setType == SET_OVERLINE) {
        setOverline(value);
    } else if (setType == SET_UNDERLINE) {
        setUnderline(value);
    } else if (setType == SET_STRIKEOUT) {
        setStrikeOut(value);
    }
}

MindMapObject* MindMapObject::newObj()
{
    if (m_newObj == nullptr) {
        m_newObj = new MindMapObject(nullptr);
        m_newObj->m_srcObj = this;
    }
    return m_newObj;
}

MindMapObject* MindMapObject::srcObj()
{
    return m_srcObj;
}

bool MindMapObject::value(int setType)
{
    if (setType == SET_BOLD) {
        return bold();
    } else if (setType == SET_ITALICS) {
        return italics();
    } else if (setType == SET_OVERLINE) {
        return overline();
    } else if (setType == SET_UNDERLINE) {
        return underline();
    } else if (setType == SET_STRIKEOUT) {
        return strikOut();
    }
    return false;
}
