#ifndef OPENPROJECTDIALOG_H
#define OPENPROJECTDIALOG_H

#include "mydao.h"
#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class OpenProjectDialog;
}

class OpenProjectDialog : public QDialog {
    Q_OBJECT

public:
    explicit OpenProjectDialog(QWidget* parent = nullptr);
    ~OpenProjectDialog();
    void loadProjects();
    void setMyDao(MyDao* myDao);
    int id();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::OpenProjectDialog* ui;
    MyDao* m_myDao;
};

#endif // OPENPROJECTDIALOG_H
