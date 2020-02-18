#ifndef EDITLINKDIALOG_H
#define EDITLINKDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class EditLinkDialog;
}

class EditLinkDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditLinkDialog(QWidget* parent = nullptr);
    ~EditLinkDialog();
    void setLink(QString link);
    QString link();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_btnOpen_clicked();

private:
    Ui::EditLinkDialog* ui;
};

#endif // EDITLINKDIALOG_H
