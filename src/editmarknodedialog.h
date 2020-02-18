#ifndef EDITMARKNODEDIALOG_H
#define EDITMARKNODEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QSet>

namespace Ui {
class EditMarkNodeDialog;
}

class EditMarkNodeDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditMarkNodeDialog(QWidget* parent = nullptr);
    ~EditMarkNodeDialog();
    void setMarkNodes(QSet<int>& markNodes);
    QSet<int>& markNodes();
private slots:
    void on_lvData_itemDoubleClicked(QListWidgetItem* item);

    void on_lvSel_itemDoubleClicked(QListWidgetItem* item);

    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::EditMarkNodeDialog* ui;
    QSet<int> m_markNodes;
};

#endif // EDITMARKNODEDIALOG_H
