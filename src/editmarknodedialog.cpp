#include "editmarknodedialog.h"
#include "ui_editmarknodedialog.h"

EditMarkNodeDialog::EditMarkNodeDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditMarkNodeDialog)
{
    ui->setupUi(this);
    ui->lvData->setViewMode(QListWidget::IconMode);
    ui->lvSel->setViewMode(QListWidget::IconMode);
    for (int i = 1; i <= 19; i++) {
        QListWidgetItem* item = new QListWidgetItem();
        item->setIcon(QIcon(QString(":/res/flags/%1.png").arg(i)));
        item->setData(Qt::UserRole, i);
        ui->lvData->addItem(item);
    }
}

EditMarkNodeDialog::~EditMarkNodeDialog()
{
    delete ui;
}

void EditMarkNodeDialog::setMarkNodes(QSet<int>& markNodes)
{
    m_markNodes = markNodes;
    ui->lvSel->clear();
    for (auto i : m_markNodes) {
        QListWidgetItem* item = new QListWidgetItem();
        item->setIcon(QIcon(QString(":/res/flags/%1.png").arg(i)));
        item->setData(Qt::UserRole, i);
        ui->lvSel->addItem(item);
    }
}

void EditMarkNodeDialog::on_lvData_itemDoubleClicked(QListWidgetItem* item)
{
    int i = item->data(Qt::UserRole).toInt();
    if (!m_markNodes.contains(i)) {
        m_markNodes << i;
        QListWidgetItem* item = new QListWidgetItem();
        item->setIcon(QIcon(QString(":/res/flags/%1.png").arg(i)));
        item->setData(Qt::UserRole, i);
        ui->lvSel->addItem(item);
    }
}

void EditMarkNodeDialog::on_lvSel_itemDoubleClicked(QListWidgetItem* item)
{
    int i = item->data(Qt::UserRole).toInt();
    m_markNodes.remove(i);
    ui->lvSel->removeItemWidget(item);
    delete item;
}

void EditMarkNodeDialog::on_btnCancel_clicked()
{
    reject();
}

void EditMarkNodeDialog::on_btnOk_clicked()
{
    accept();
}

QSet<int>& EditMarkNodeDialog::markNodes()
{
    return m_markNodes;
}
