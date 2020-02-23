#include "editcolordialog.h"
#include "ui_editcolordialog.h"

EditColorDialog::EditColorDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditColorDialog)
{
    ui->setupUi(this);
    m_colorIndex = -1;
    ui->lvRecent->setViewMode(QListWidget::IconMode);
    ui->lvData->setViewMode(QListWidget::IconMode);
    ColorTable* colorTable = ColorTable::getInstance();
    QList<QColor> colors = colorTable->colors();
    for (int i = 0; i < colors.count(); i++) {
        ColorWidget* w = new ColorWidget(ui->lvData);
        w->setColor(colors.at(i));
        w->setIndex(i);
        connect(w, &ColorWidget::onClick, this, &EditColorDialog::onColorWidgetClick);
        QListWidgetItem* item = new QListWidgetItem(ui->lvData);
        item->setData(Qt::UserRole, i);
        item->setSizeHint(QSize(32, 32));
        ui->lvData->addItem(item);
        ui->lvData->setItemWidget(item, w);
    }
    for (int i : ColorTable::colorIndexes) {
        ColorWidget* w = new ColorWidget(ui->lvRecent);
        w->setColor(colors.at(i));
        w->setIndex(i);
        connect(w, &ColorWidget::onClick, this, &EditColorDialog::onColorWidgetClick);
        QListWidgetItem* item = new QListWidgetItem(ui->lvRecent);
        item->setData(Qt::UserRole, i);
        item->setSizeHint(QSize(32, 32));
        ui->lvRecent->addItem(item);
        ui->lvRecent->setItemWidget(item, w);
    }
}

EditColorDialog::~EditColorDialog()
{
    delete ui;
}

int EditColorDialog::colorIndex()
{
    ColorTable::addColorIndex(m_colorIndex);
    emit onAddColorIndex(m_colorIndex);
    return m_colorIndex;
}

void EditColorDialog::on_btnCancel_clicked()
{
    reject();
}

void EditColorDialog::on_btnOk_clicked()
{
    accept();
}

void EditColorDialog::onColorWidgetClick(int index, QColor color)
{
    m_colorIndex = index;
}
