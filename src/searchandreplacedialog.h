#ifndef SEARCHANDREPLACEDIALOG_H
#define SEARCHANDREPLACEDIALOG_H

#include "mindmapwidget.h"
#include <QDialog>
namespace Ui {
class SearchAndReplaceDialog;
}
class MindMapWidget;
class SearchAndReplaceDialog : public QDialog {
    Q_OBJECT

public:
    explicit SearchAndReplaceDialog(QWidget* parent = nullptr);
    ~SearchAndReplaceDialog();
    void setMindMapWidget(MindMapWidget* mindmapwidget);

private slots:
    void on_btnSearch_clicked();

    void on_lvData_itemClicked(QListWidgetItem* item);

    void on_btnReplace_clicked();

private:
    Ui::SearchAndReplaceDialog* ui;
    MindMapWidget* m_mindmapwidget;
};

#endif // SEARCHANDREPLACEDIALOG_H
