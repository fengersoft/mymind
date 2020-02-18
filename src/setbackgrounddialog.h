#ifndef SETBACKGROUNDDIALOG_H
#define SETBACKGROUNDDIALOG_H

#include <QDialog>
#include <QGraphicsView>
#include <QtDebug>

namespace Ui {
class SetBackGroundDialog;
}

class SetBackGroundDialog : public QDialog {
    Q_OBJECT

public:
    explicit SetBackGroundDialog(QWidget* parent = nullptr);
    ~SetBackGroundDialog();
    void setBid(int bid);
    int bid();

private slots:
    void on_cbbSel_currentIndexChanged(int index);

    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::SetBackGroundDialog* ui;
    QGraphicsScene* m_scence;
    int m_bid;
};

#endif // SETBACKGROUNDDIALOG_H
