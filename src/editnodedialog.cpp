#include "editnodedialog.h"
#include "ui_editnodedialog.h"

EditNodeDialog::EditNodeDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditNodeDialog)
{
    ui->setupUi(this);
}

EditNodeDialog::~EditNodeDialog()
{
    delete ui;
}

QString EditNodeDialog::getName()
{
    return ui->edtName->text();
}

void EditNodeDialog::setName(QString name)
{
    ui->edtName->setText(name);
}

void EditNodeDialog::on_btnCancel_clicked()
{
    reject();
}

void EditNodeDialog::on_btnOk_clicked()
{
    accept();
}
