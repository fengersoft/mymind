#include "searchandreplacedialog.h"
#include "ui_searchandreplacedialog.h"

SearchAndReplaceDialog::SearchAndReplaceDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SearchAndReplaceDialog)
{
    ui->setupUi(this);
}

SearchAndReplaceDialog::~SearchAndReplaceDialog()
{
    delete ui;
}

void SearchAndReplaceDialog::setMindMapWidget(MindMapWidget* mindmapwidget)
{
    m_mindmapwidget = mindmapwidget;
}

void SearchAndReplaceDialog::on_btnSearch_clicked()
{
    if (ui->edtSearch->text().trimmed() == "") {
        return;
    }
    ui->lvData->clear();
    for (int i = 0; i < m_mindmapwidget->mindMapObjects().count(); i++) {
        MindMapObject* obj = m_mindmapwidget->mindMapObjects().at(i);
        if (obj->name().indexOf(ui->edtSearch->text()) >= 0) {
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(obj->name());
            item->setData(Qt::UserRole, m_mindmapwidget->mindMapObjects().indexOf(obj));
            ui->lvData->addItem(item);
        }
    }
}

void SearchAndReplaceDialog::on_lvData_itemClicked(QListWidgetItem* item)
{
    int i = item->data(Qt::UserRole).toInt();
    MindMapObject* obj = m_mindmapwidget->mindMapObjects().at(i);
    m_mindmapwidget->setSelObject(obj);
}

void SearchAndReplaceDialog::on_btnReplace_clicked()
{
    if (ui->edtSearch->text().trimmed() == "") {
        return;
    }
    ui->lvData->clear();
    for (int i = 0; i < m_mindmapwidget->mindMapObjects().count(); i++) {
        MindMapObject* obj = m_mindmapwidget->mindMapObjects().at(i);
        if (obj->name().indexOf(ui->edtSearch->text()) >= 0) {
            QListWidgetItem* item = new QListWidgetItem();
            QString s0 = obj->name();
            QString s1 = s0;
            s1.replace(ui->edtSearch->text(), ui->edtReplace->text());
            obj->setName(s1);
            m_mindmapwidget->myDao()->editNode(obj->id(), s1);
            item->setText("\"" + s0 + "\"替换为\"" + obj->name() + "\"");
            item->setData(Qt::UserRole, m_mindmapwidget->mindMapObjects().indexOf(obj));
            ui->lvData->addItem(item);
        }
    }
    m_mindmapwidget->update();
}
