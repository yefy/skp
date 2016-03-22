#ifndef SKP_MAINWINDOW_H
#define SKP_MAINWINDOW_H

#include <QMainWindow>
#include "skp_change_widget.h"
#include "skp_themed_base.h"

#include "skp_utility.h"
#include <QSystemTrayIcon>

class NcFramelessHelper;
class SkpFileWidget;
class SkpSocketTestWidget;
class QSystemTrayIcon;
class QLabel;

namespace Ui {
class SkpMainWindow;
}

class SkpMainWindow : public QMainWindow, public SkpChangeWidget, public SkpThemedBase
{
    Q_OBJECT

public:
    explicit SkpMainWindow(QWidget *parent = 0);
    ~SkpMainWindow();
    void skp_show();

protected:
    void mouseDoubleClickEvent(QMouseEvent *mouseEvent);
    void paintEvent(QPaintEvent *paintEvent);
    void showEvent(QShowEvent *showEvent);
    void resizeEvent(QResizeEvent *resizeEvent);
    void changeEvent(QEvent *event);

private:
    Ui::SkpMainWindow *ui;
    QSystemTrayIcon *m_systemTrayIcon;
    QLabel *m_statusLabel;
    NcFramelessHelper *m_framelessHelper;

    SkpFileWidget *m_fileWidget;
    SkpSocketTestWidget *m_socketTestWidget;
    SkpSocketTestWidget *m_socketTestWidget_2;

private slots:
    void skp_on_skin();
    void skp_on_min();
    void skp_on_max();
    void skp_on_close();
    void skp_on_doubleClick();
    void skp_on_activated(QSystemTrayIcon::ActivationReason reason);
    void skp_on_Show();

private:
    void addTabWidget(QWidget *widget, const QPixmap &pixmap, const QString &text);
};

#endif // SKP_MAINWINDOW_H
