#include "openprojectdialog.h"
#include "ui_openprojectdialog.h"

OpenProjectDialog::OpenProjectDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::OpenProjectDialog)
{
    ui->setupUi(this);
}

OpenProjectDialog::~OpenProjectDialog()
{
    delete ui;
}

void OpenProjectDialog::loadProjects()
{

    QSqlQuery qry;
    m_myDao->sqliteWrapper->select("select name,id from mind_data where pid=-1 order by pid", qry);
    ui->cbbProjects->clear();
    while (qry.next()) {
        ui->cbbProjects->addItem(qry.value(0).toString(), qry.value(1).toInt());
    }
}

void OpenProjectDialog::setMyDao(MyDao* myDao)
{
    m_myDao = myDao;
    loadProjects();
}

int OpenProjectDialog::id()
{
    int id = ui->cbbProjects->currentData().toInt();
    return id;
}

void OpenProjectDialog::on_btnCancel_clicked()
{
    reject();
}

void OpenProjectDialog::on_btnOk_clicked()
{
    accept();
}
