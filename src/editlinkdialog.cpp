#include "editlinkdialog.h"
#include "ui_editlinkdialog.h"

EditLinkDialog::EditLinkDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditLinkDialog)
{
    ui->setupUi(this);
}

EditLinkDialog::~EditLinkDialog()
{
    delete ui;
}

void EditLinkDialog::setLink(QString link)
{
    ui->cbbLink->setCurrentText(link);
}

QString EditLinkDialog::link()
{
    QString url = ui->cbbLink->currentText();
    url = url.replace("\\", "/");
    return url;
}

void EditLinkDialog::on_btnCancel_clicked()
{
    reject();
}

void EditLinkDialog::on_btnOk_clicked()
{
    accept();
}

void EditLinkDialog::on_btnOpen_clicked()
{
    QString path = QFileDialog::getOpenFileName();
    if (path == "") {
        return;
    }
    ui->cbbLink->setCurrentText(path);
}
