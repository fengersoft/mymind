#include "editremarkdialog.h"
#include "ui_editremarkdialog.h"

EditRemarkDialog::EditRemarkDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditRemarkDialog)
{
    ui->setupUi(this);
}

EditRemarkDialog::~EditRemarkDialog()
{
    delete ui;
}

void EditRemarkDialog::setRemark(QString remark)
{
    ui->edtRemark->setPlainText(remark);
}

QString EditRemarkDialog::remark()
{
    return ui->edtRemark->toPlainText();
}

void EditRemarkDialog::on_btnCancel_clicked()
{
    reject();
}

void EditRemarkDialog::on_btnOk_clicked()
{
    accept();
}
