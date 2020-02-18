#ifndef EDITNODEDIALOG_H
#define EDITNODEDIALOG_H

#include <QDialog>

namespace Ui {
class EditNodeDialog;
}

class EditNodeDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditNodeDialog(QWidget* parent = nullptr);
    ~EditNodeDialog();
    QString getName();
    void setName(QString name);

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::EditNodeDialog* ui;
};

#endif // EDITNODEDIALOG_H
