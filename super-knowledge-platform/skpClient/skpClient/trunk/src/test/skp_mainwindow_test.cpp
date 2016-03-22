#include "skp_mainwindow_test.h"
#include "ui_skp_mainwindow_test.h"

#include "skp_socket_test_widget.h"

SkpMainwindowTest::SkpMainwindowTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SkpMainwindowTest)
{
    ui->setupUi(this);

    connect(ui->socketTestPushButton, SIGNAL(clicked()),
            this, SLOT(skp_on_socket_test()));
}

SkpMainwindowTest::~SkpMainwindowTest()
{
    delete ui;
}

void SkpMainwindowTest::skp_on_socket_test()
{
    SkpSocketTestWidget *socketTestWidget = new SkpSocketTestWidget();
    socketTestWidget->show();
}
