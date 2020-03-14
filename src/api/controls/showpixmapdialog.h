#ifndef SHOWPIXMAPDIALOG_H
#define SHOWPIXMAPDIALOG_H

#include "pixmapviewer.h"
#include <QDialog>
#include <QHBoxLayout>
#include <QPixmap>

namespace Ui {
class showPixmapDialog;
}

class showPixmapDialog : public QDialog {
    Q_OBJECT

public:
    explicit showPixmapDialog(QWidget* parent = nullptr);
    ~showPixmapDialog();
    void setImage(QPixmap& image);

private:
    Ui::showPixmapDialog* ui;
    PixmapViewer* m_viewer;
};

#endif // SHOWPIXMAPDIALOG_H
