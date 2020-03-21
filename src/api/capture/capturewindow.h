#ifndef CAPTUREWINDOW_H
#define CAPTUREWINDOW_H

#include <QAction>
#include <QClipboard>
#include <QDesktopWidget>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QtDebug>
#include <math.h>
namespace Ui {
class CaptureWindow;
}

class CaptureWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit CaptureWindow(QWidget* parent = nullptr);
    ~CaptureWindow();

    QPixmap pix() const;
    void setPix(const QPixmap& pix);
    void getSelectRect();
public slots:
    void onMenuTriggered(bool checked = false);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::CaptureWindow* ui;
    QPixmap m_pix;
    QPoint m_mousedownPoint;
    bool m_mousedownFlag;
    QRect m_selectRc;
    QRect m_pixRc;
};
void startShootScreen(bool hideWindow = true);

#endif // CAPTUREWINDOW_H
