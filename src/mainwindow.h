#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mindmapwidget.h"
#include "mydao.h"
#include "openprojectdialog.h"
#include <QAction>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenu>
#include <QTimer>

#include "aboutdialog.h"
#include "editlinkdialog.h"
#include "editnodedialog.h"
#include "editremarkdialog.h"
#include "mindmapwidgettoolbar.h"
#include "openrecentdialog.h"
#include "setbackgrounddialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    MindMapWidget* mindMapWidget;
    MindMapWidgetToolBar* mindMapWidgetToolBar;
    MyDao* myDao;
    void openDefaultProject();
    void addNewProject();
    void showOpenProjectDialog();
    void deleteProject();
    void openRecentProject();

    QTimer* tmr;
private slots:

    void on_btnEdit_clicked();

    void on_btnMenu_clicked();
    void onMenuItemTriggered(bool checked = false);
    void ontmrTimeOut();

    void on_btnRemark_clicked();

    void on_btnLink_clicked();

    void on_btnFlag_clicked();

private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
