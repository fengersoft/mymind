#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myDao = new MyDao(this);
    mindMapWidget = new MindMapWidget(this);

    QHBoxLayout* lay = new QHBoxLayout();
    ui->wgtClient->setLayout(lay);
    lay->setMargin(0);
    lay->addWidget(mindMapWidget);
    mindMapWidget->setMyDao(myDao);
    connect(mindMapWidget, &MindMapWidget::onDoubleClick, this, &MainWindow::on_btnEdit_clicked);
    mindMapWidgetToolBar = new MindMapWidgetToolBar(ui->wgtMindMapToolbar);
    QHBoxLayout* laytool = new QHBoxLayout();
    ui->wgtMindMapToolbar->setLayout(laytool);
    laytool->setMargin(0);
    laytool->addWidget(mindMapWidgetToolBar);

    openDefaultProject();
    showMaximized();
    tmr = new QTimer(this);
    connect(tmr, &QTimer::timeout, this, &MainWindow::ontmrTimeOut);
    tmr->start(50);
}

MainWindow::~MainWindow()
{
    ColorTable::freeColorTable();
    delete ui;
}

void MainWindow::openDefaultProject()
{
    QSqlQuery qry;
    QString sql = "select pid from mind_history order by id desc";
    myDao->sqliteWrapper->select(sql, qry);
    if (qry.next()) {
        int pid = qry.value(0).toInt();
        mindMapWidget->openProject(pid);
    }
}

void MainWindow::addNewProject()
{
    EditNodeDialog* dlg = new EditNodeDialog();
    dlg->setWindowTitle("新建导图");
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        QString name = dlg->getName();
        int id = myDao->addNode(-1, name, 1);
        mindMapWidget->openProject(id);
    }
    delete dlg;
}

void MainWindow::showOpenProjectDialog()
{
    OpenProjectDialog* dlg = new OpenProjectDialog();
    dlg->setMyDao(myDao);
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        mindMapWidget->openProject(dlg->id());
    }
}

void MainWindow::deleteProject()
{
    if (mindMapWidget->projectId() == -1) {
        return;
    }
    int ret = QMessageBox::question(this, "提示", "确定删除当前导图吗?");
    if (ret == QMessageBox::No) {
        return;
    }
    mindMapWidget->deleteProject();
}

void MainWindow::openRecentProject()
{
    OpenRecentDialog* dlg = new OpenRecentDialog();
    dlg->setMyDao(myDao);
    int ret = dlg->exec();

    if (ret == QDialog::Accepted) {
        int pid = dlg->pid();
        if (pid != -1) {
            mindMapWidget->openProject(pid);
        }
    }

    delete dlg;
}

void MainWindow::on_btnEdit_clicked()
{
    MindMapObject* selObj = mindMapWidget->selObject();
    if (selObj == nullptr) {
        return;
    }
    EditNodeDialog* dlg = new EditNodeDialog();
    dlg->setName(selObj->name());
    dlg->setWindowTitle("修改");
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        QString name = dlg->getName();
        myDao->editNode(selObj->id(), name);
        mindMapWidget->editNode(selObj, name);
    }
    delete dlg;
}

void MainWindow::on_btnMenu_clicked()
{
    QPoint pt = QPoint(0, ui->btnMenu->height());
    pt = ui->btnMenu->mapToGlobal(pt);
    QMenu* menu = new QMenu();
    QStringList menuNames;
    menuNames << "新建导图"
              << "打开导图"
              << "最近导图"
              << "查看导图大纲"
              << "删除导图"
              << "设置导图背景"
              << "导出导图为图片"
              << "关于导图";
    for (int i = 0; i < menuNames.count(); i++) {
        QAction* act = new QAction(menu);
        connect(act, &QAction::triggered, this, &MainWindow::onMenuItemTriggered);
        act->setText(menuNames[i]);
        menu->addAction(act);
    }
    menu->exec(pt);
    delete menu;
}

void MainWindow::onMenuItemTriggered(bool checked)
{
    QAction* act = static_cast<QAction*>(sender());
    if (act->text() == "新建导图") {
        addNewProject();
    } else if (act->text() == "打开导图") {
        showOpenProjectDialog();
    } else if (act->text() == "删除导图") {
        deleteProject();
    } else if (act->text() == "最近导图") {
        openRecentProject();
    } else if (act->text() == "导出导图为图片") {
        mindMapWidget->saveDataAsPng();
    } else if (act->text() == "设置导图背景") {
        if (mindMapWidget->projectId() == -1) {
            return;
        }
        SetBackGroundDialog* dlg = new SetBackGroundDialog();
        int ret = dlg->exec();
        if (ret == QDialog::Accepted) {
            mindMapWidget->setBackGroundId(dlg->bid());
            myDao->sqliteWrapper->execute(QString("delete from mind_background where pid=%1").arg(mindMapWidget->projectId()));
            myDao->sqliteWrapper->execute(QString("insert into mind_background (pid,bid) values (%1,%2)").arg(mindMapWidget->projectId()).arg(dlg->bid()));
        }
        delete dlg;
    } else if (act->text() == "查看导图大纲") {
        mindMapWidget->showMindLineOut();
    } else if (act->text() == "关于导图") {
        AboutDialog* dlg = new AboutDialog();
        dlg->exec();
        delete dlg;
    }
}

void MainWindow::ontmrTimeOut()
{
    ui->btnEdit->setEnabled(mindMapWidget->projectId() != -1);
    ui->btnRemark->setEnabled(mindMapWidget->projectId() != -1);
    ui->btnLink->setEnabled(mindMapWidget->projectId() != -1);
}

void MainWindow::on_btnRemark_clicked()
{
    MindMapObject* selObj = mindMapWidget->selObject();
    if (selObj == nullptr) {
        return;
    }
    EditRemarkDialog* dlg = new EditRemarkDialog();
    dlg->setRemark(selObj->remark());
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        selObj->setRemark(dlg->remark());
        myDao->sqliteWrapper->execute(QString("update mind_data set remark='%1' where id=%2").arg(dlg->remark()).arg(selObj->id()));
        update();
    }
    delete dlg;
}

void MainWindow::on_btnLink_clicked()
{
    MindMapObject* selObj = mindMapWidget->selObject();
    if (selObj == nullptr) {
        return;
    }
    EditLinkDialog* dlg = new EditLinkDialog();
    dlg->setLink(selObj->link());
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        selObj->setLink(dlg->link());
        myDao->sqliteWrapper->execute(QString("update mind_data set link='%1' where id=%2").arg(dlg->link()).arg(selObj->id()));
        update();
    }
    delete dlg;
}

void MainWindow::on_btnFlag_clicked()
{
    mindMapWidget->showEditMarkNodeDialog();
}
