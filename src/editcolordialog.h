#ifndef EDITCOLORDIALOG_H
#define EDITCOLORDIALOG_H

#include "api/color/colortable.h"
#include "api/controls/colorwidget.h"
#include <QDialog>
#include <QListWidget>
#include <QtDebug>

namespace Ui {
class EditColorDialog;
}

class EditColorDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditColorDialog(QWidget* parent = nullptr);
    ~EditColorDialog();
    int colorIndex();
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void onColorWidgetClick(int index, QColor color);

private:
    Ui::EditColorDialog* ui;
    int m_colorIndex = -1;
};

#endif // EDITCOLORDIALOG_H
