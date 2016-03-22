#ifndef SKP_MAINWINDOW_TEST_H
#define SKP_MAINWINDOW_TEST_H

#include <QMainWindow>

class SkpSocketTestWidget;

namespace Ui {
class SkpMainwindowTest;
}

class SkpMainwindowTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit SkpMainwindowTest(QWidget *parent = 0);
    ~SkpMainwindowTest();

private:
    Ui::SkpMainwindowTest *ui;

private slots:
    void skp_on_socket_test();

private:
    //SkpSocketTestWidget *m_socketTestWidget;
};

#endif // SKP_MAINWINDOW_TEST_H
