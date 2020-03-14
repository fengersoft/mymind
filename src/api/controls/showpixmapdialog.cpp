#include "showpixmapdialog.h"
#include "ui_showpixmapdialog.h"

showPixmapDialog::showPixmapDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::showPixmapDialog)
{
    ui->setupUi(this);
    QHBoxLayout* lay = new QHBoxLayout();
    this->setLayout(lay);
    lay->setContentsMargins(0, 0, 0, 0);
    m_viewer = new PixmapViewer(this);
    lay->addWidget(m_viewer);
}

showPixmapDialog::~showPixmapDialog()
{
    delete ui;
}

void showPixmapDialog::setImage(QPixmap& image)
{
    m_viewer->setImage(image);
}
