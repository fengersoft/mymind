#include "showmindlineoutdialog.h"
#include "ui_showmindlineoutdialog.h"

ShowMindLineOutDialog::ShowMindLineOutDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ShowMindLineOutDialog)
{
    ui->setupUi(this);
}

ShowMindLineOutDialog::~ShowMindLineOutDialog()
{
    delete ui;
}

void ShowMindLineOutDialog::setInfos(QStringList& infos)
{
    ui->edtInfo->setPlainText(infos.join("\n"));
}

void ShowMindLineOutDialog::on_btnClose_clicked()
{
    close();
}
