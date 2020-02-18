#include "setbackgrounddialog.h"
#include "ui_setbackgrounddialog.h"

SetBackGroundDialog::SetBackGroundDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SetBackGroundDialog)
{
    ui->setupUi(this);
    m_scence = new QGraphicsScene(ui->imgView);
    ui->imgView->setScene(m_scence);

    ui->cbbSel->addItem("无背景", 0);
    for (int i = 1; i <= 10; i++) {
        ui->cbbSel->addItem(QString("背景%1").arg(i), i);
    }
}

SetBackGroundDialog::~SetBackGroundDialog()
{
    delete ui;
}

void SetBackGroundDialog::setBid(int bid)
{
    m_bid = bid;
    ui->cbbSel->setCurrentIndex(m_bid);
}

int SetBackGroundDialog::bid()
{
    return m_bid;
}

void SetBackGroundDialog::on_cbbSel_currentIndexChanged(int index)
{

    m_bid = index;
    m_scence->clear();
    if (index > 0) {
        QPixmap pix = QPixmap(QString(":/res/background/%1.jpg").arg(index));
        pix = pix.scaledToHeight(ui->imgView->height() - 32, Qt::SmoothTransformation);
        m_scence->addPixmap(pix);
    }
}

void SetBackGroundDialog::on_btnOk_clicked()
{
    accept();
}

void SetBackGroundDialog::on_btnCancel_clicked()
{
    reject();
}
