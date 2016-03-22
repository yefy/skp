#ifndef SKP_LOGIN_MAIN_WIDGET_H
#define SKP_LOGIN_MAIN_WIDGET_H

#include <QWidget>

class SkpLoginWidget;
class SkpSignal;
class SkpMainWindow;

namespace Ui {
class SkpLoginMainWidget;
}

class SkpLoginMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SkpLoginMainWidget(SkpLoginWidget *parent = 0);
    ~SkpLoginMainWidget();
    SkpLoginWidget *m_loginWidget;

private:
    Ui::SkpLoginMainWidget *ui;
    SkpSignal *m_signal;
    SkpMainWindow *m_mainWindow;

private:
    void createMysqlDB();
private slots:
    void onLogin();
    void onLoginServer(int loginStatus);
signals:
    void sigLoginServer(int loginStatus);
};

#endif // SKP_LOGIN_MAIN_WIDGET_H
