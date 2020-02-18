#ifndef OPENRECENTDIALOG_H
#define OPENRECENTDIALOG_H

#include "mydao.h"
#include <QDialog>
#include <QListWidget>
#include <QSqlQuery>

namespace Ui {
class OpenRecentDialog;
}

class OpenRecentDialog : public QDialog {
    Q_OBJECT

public:
    explicit OpenRecentDialog(QWidget* parent = nullptr);
    ~OpenRecentDialog();
    void setMyDao(MyDao* mydao);
    int pid();

private slots:
    void on_lvData_itemClicked(QListWidgetItem* item);

    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::OpenRecentDialog* ui;
    MyDao* m_mydao;
    int m_pid;
};

#endif // OPENRECENTDIALOG_H
