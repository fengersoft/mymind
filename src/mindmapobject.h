#ifndef MINDMAPOBJECT_H
#define MINDMAPOBJECT_H

#include "api/color/colortable.h"
#include <QColor>
#include <QObject>
#include <QRect>
#include <QSet>
#include <QtDebug>
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
signals:

public slots:
};

#endif // MINDMAPOBJECT_H
