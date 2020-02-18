#ifndef SHOWMINDLINEOUTDIALOG_H
#define SHOWMINDLINEOUTDIALOG_H

#include <QDialog>

namespace Ui {
class ShowMindLineOutDialog;
}

class ShowMindLineOutDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShowMindLineOutDialog(QWidget* parent = nullptr);
    ~ShowMindLineOutDialog();
    void setInfos(QStringList& infos);

private slots:
    void on_btnClose_clicked();

private:
    Ui::ShowMindLineOutDialog* ui;
};

#endif // SHOWMINDLINEOUTDIALOG_H
