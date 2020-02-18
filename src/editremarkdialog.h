#ifndef EDITREMARKDIALOG_H
#define EDITREMARKDIALOG_H

#include <QDialog>

namespace Ui {
class EditRemarkDialog;
}

class EditRemarkDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditRemarkDialog(QWidget* parent = nullptr);
    ~EditRemarkDialog();
    void setRemark(QString remark);
    QString remark();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::EditRemarkDialog* ui;
};

#endif // EDITREMARKDIALOG_H
