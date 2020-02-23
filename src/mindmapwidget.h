#ifndef MINDMAPWIDGET_H
#define MINDMAPWIDGET_H

#include "editcolordialog.h"
#include "editmarknodedialog.h"
#include "editnodedialog.h"
#include "mindmapobject.h"
#include "mydao.h"
#include "searchandreplacedialog.h"
#include "showmindlineoutdialog.h"
#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QDesktopServices>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QSqlQuery>
#include <QWidget>

namespace Ui {
class MindMapWidget;
}
class SearchAndReplaceDialog;
class MindMapWidget : public QWidget {
    Q_OBJECT

public:
    explicit MindMapWidget(QWidget* parent = nullptr);
    ~MindMapWidget();
    ///打开导图
    void openProject(int id);
    void paintEvent(QPaintEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void drawRemark(QPainter& painter);
    void clear();
    void addNode(int pid, int id = 0);
    void addNodeInfo(MindMapObject* obj, int i, QSqlQuery& qry);
    MindMapObject* addChildNode(MindMapObject* pobj, int id, QString name);
    void setMyDao(MyDao* myDao);
    void drawNode(int pid, int px, int py, int x, int y, QPainter& painter);
    void mousePressEvent(QMouseEvent* event);
    MindMapObject* selObject();
    void setSelObject(MindMapObject* obj);
    int getNodeNeedCount(MindMapObject* pobj);
    void getMindWidgetChildNum(MindMapObject* pobj);
    void getMindWidgetsChildNum();
    void getNodeCount(MindMapObject* pobj, int& n);
    void mouseReleaseEvent(QMouseEvent* event);
    void getChilds(int pid, QList<int>& ids);
    void wheelEvent(QWheelEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void editNode(MindMapObject* selObj, QString name);
    void delSelObject();
    void deleteNodeById(int id);
    int projectId();
    void deleteProject();
    void showSelPopMenu();
    void showPopMenu();
    void releaseMouseFlag();
    void showEditMarkNodeDialog();
    ///为结点增加图标
    void addMarkNodes(MindMapObject* obj);
    void saveDataAsPng();
    void saveDataAsMarkdown();
    void saveDataAsTxtFile();
    void drawImage(QPaintDevice* paintDevice, int imgWidth, int imgHeight, int factor = 1);
    void getDrawRect(int& w, int& h);
    void reSetOrder(int pid);
    void setBackGroundId(int backGroundId);
    int backGroundId();
    void onAddChildNodeEvent();
    void onAddChildNodeEvent(QString name);
    int getNewMaxSxh(int pid);
    void showMindLineOut();
    void showMindLineOutInfo(int pid, QString prefix, QStringList& infos, QString sign = "·", QString space = "");
    void getSelectRect(QRect& rc, QPoint pt1, QPoint pt2);
    QColor numColor(int sxh);
    void setNodeFontStyle(int setType);
    void setFontStyle();
    void applyFontStyle();
    int b2i(bool b);
    void setNodeNameFont(MindMapObject* obj, QPainter& painter, QFont& font);
    void setFontAsDefault(QFont& font);
    void showBackColorEditDialog();
    void showFontColorEditDialog();
    void saveMindMapAsNewProject();
    QList<MindMapObject*>& mindMapObjects();
    MyDao* myDao();
public slots:
    void onPopMenuTrigger();
    void onAddColorIndex(int colorIndex);
Q_SIGNALS:
    void onDoubleClick();

private:
    Ui::MindMapWidget* ui;
    QList<MindMapObject*> m_mindMapObjects;
    MyDao* m_myDao;
    MindMapObject* m_selObject;
    MindMapObject* m_cutObject;
    QPoint m_rootPoint;
    bool m_mousedownFlag;
    QPoint m_mousedownPoint;
    int m_projectId;
    int getMaxLen(QStringList& stl);
    int m_backgroundId;
    QPixmap m_backGroundPix;
    bool m_screenshotFlag;
    QPoint m_startShot;
    void loadBackground();
    int getQryValue(QSqlQuery& qry, int index, int defaultValue);
    int getQryValue(QSqlQuery& qry, QString fieldName, int defaultValue);
    bool getQryValueBool(QSqlQuery& qry, QString fieldName, bool defaultValue = false);
};

#endif // MINDMAPWIDGET_H
