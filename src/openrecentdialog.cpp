#include "openrecentdialog.h"
#include "ui_openrecentdialog.h"

OpenRecentDialog::OpenRecentDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::OpenRecentDialog)
{
    ui->setupUi(this);
    m_pid = -1;
}

OpenRecentDialog::~OpenRecentDialog()
{
    delete ui;
}

void OpenRecentDialog::setMyDao(MyDao* mydao)
{
    m_mydao = mydao;
    QSqlQuery qry;
    QString sql = "select b.name,a.pid from mind_history a left join mind_data b on a.pid=b.id  where b.name is not null order by a.id desc";
    m_mydao->sqliteWrapper->select(sql, qry);
    QSet<int> ids;
    while (qry.next()) {
        int pid = qry.value(1).toInt();
        if (!ids.contains(pid)) {
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(qry.value(0).toString());
            item->setData(Qt::UserRole, qry.value(1).toInt());
            ui->lvData->addItem(item);
            ids << pid;
        }
    }
}

int OpenRecentDialog::pid()
{
    return m_pid;
}

void OpenRecentDialog::on_lvData_itemClicked(QListWidgetItem* item)
{
    m_pid = item->data(Qt::UserRole).toInt();
}

void OpenRecentDialog::on_btnCancel_clicked()
{
    reject();
}

void OpenRecentDialog::on_btnOk_clicked()
{
    accept();
}
