#ifndef MINDMAPOBJECT_H
#define MINDMAPOBJECT_H

#include "api/color/colortable.h"
#include <QColor>
#include <QObject>
#include <QPixmap>
#include <QRect>
#include <QSet>
#include <QtDebug>
const int SET_BOLD = 0;
const int SET_ITALICS = 1;
const int SET_UNDERLINE = 2;
const int SET_OVERLINE = 3;
const int SET_STRIKEOUT = 4;
const int SET_FONT_BACKCOLOR = 5;
const int SET_FONT_COLOR = 6;
class MindMapObject : public QObject {
    Q_OBJECT
public:
    explicit MindMapObject(QObject* parent = nullptr);
    void setPid(int pid);
    int id();
    int pid();
    void setId(int id);
    void setName(QString name);
    QString name();
    QRect rect;
    QRect addRect;
    int childNum;
    void setParentObj(MindMapObject* parentObj);
    MindMapObject* parentObj();
    QColor lineColor();
    QColor rootColor();
    void setRemark(QString remark);
    QString remark();
    QRect remarkRect;
    void setLink(QString link);
    QString link();
    QSet<int> markNodes;
    void setFontColor(int fontColor);
    int fontColor();
    void setBackColor(int backColor);
    int backColor();
    ~MindMapObject();
    void setSxh(int sxh);
    int sxh();
    void setShowNum(int showNum);
    bool showNum();
    void setBold(bool bold);
    bool bold();
    void setItalics(bool italics);
    bool italics();
    void setOverline(bool overline);
    bool overline();
    void setUnderline(bool underline);
    bool underline();
    void setStrikeOut(bool strikeOut);
    bool strikOut();
    bool value(int setType);
    void setValue(int setType, bool value);
    MindMapObject* newObj();
    MindMapObject* srcObj();
    void setHasImg(bool hasImg);
    bool hasImg();
    void setImg(QPixmap& img);
    QPixmap& img();

private:
    int m_pid;
    int m_id;
    QString m_name;
    MindMapObject* m_parentObj;
    QColor m_lineColor;
    QColor m_rootColor;
    QString m_remark;
    QString m_link;
    int m_fontColor;
    int m_backColor;
    int m_sxh;
    bool m_showNum;
    bool m_bold;
    bool m_italics;
    bool m_overline;
    bool m_underline;
    bool m_strikeOut;
    MindMapObject* m_newObj;
    MindMapObject* m_srcObj;
    bool m_hasImg;
    QPixmap m_img;
signals:

public slots:
};

#endif // MINDMAPOBJECT_H
