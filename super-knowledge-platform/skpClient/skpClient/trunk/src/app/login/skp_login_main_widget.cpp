#include "skp_login_main_widget.h"
#include "ui_skp_login_main_widget.h"
#include "skp_login_widget.h"
#include "skp_mainwindow.h"
#include "skp_socket_manage.h"
#include "skp_core_manage.h"
#include "skp_core_signal.h"
#include "QjtMessageBox.h"
#include "login.pb.h"
#include "skp_public_ins.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

typedef enum
{
    LoginStatusSuccess = 0,
    LoginStatusError,
}LoginStatus;

SkpLoginMainWidget::SkpLoginMainWidget(SkpLoginWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkpLoginMainWidget),
    m_loginWidget(parent)
{
    ui->setupUi(this);
    ui->user_comboBox->setEditable(true);
    ui->user_comboBox->addItem(tr("143155554942977"));
    ui->user_comboBox->addItem(tr("143155554942978"));

    ui->password_lineEdit->setText(tr("1234"));
    connect(ui->login_pushButton, SIGNAL(clicked()), this, SLOT(onLogin()));
    connect(this, SIGNAL(sigLoginServer(int)), this, SLOT(onLoginServer(int)));

    SkpSocketManage::init();
    SkpManage::instance();
    createMysqlDB();

    m_signal = new SkpSignal(NULL, 0);

//    QString tableName("id");
//    QSqlQuery query;
//    query.prepare("SELECT Fun_GetSystemIndex(?)");
//    query.addBindValue(tableName);
//    bool exec = query.exec();

//qDebug() << "exec" << exec;
//    while(query.next()){
//        quint64 id = query.value(0).toULongLong();
//        qDebug() << "id" << id;
//    }
}

SkpLoginMainWidget::~SkpLoginMainWidget()
{
    delete ui;
}

void SkpLoginMainWidget::createMysqlDB()
{
 //   QStringList drivers = QSqlDatabase::drivers();
//    foreach(QString driver, drivers)
//        qDebug() << "\t" << driver;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); // 使用mysql数据库驱动
    db.setHostName("localhost");
    db.setDatabaseName("skp_client"); // 我们之前建立的数据库
    db.setUserName("yefy"); // 直接用 yefy 用户名
    db.setPassword("yfy389"); // root 用户的密码
    //qDebug() << "MYSQL driver valid?" << db.isValid();
    //qDebug() << "=======================";
    bool ok = db.open(); // 尝试连接数据库
    assert(ok);
}

#include <QTcpSocket>
QTcpSocket *g_socket = NULL;
char g_buffer[] = "1234567890";
QString m_ip = "192.168.164.148";
quint16 m_port = 33331;

void SkpLoginMainWidget::onLogin()
{
//    if(!g_socket) {
//        g_socket = new QTcpSocket();
//        g_socket->connectToHost(m_ip, m_port);
//        g_socket->waitForConnected(3000);
//    }
//    g_socket->write(g_buffer, 10);
//    return;

    qDebug() << "start onLogin";

    QString user = ui->user_comboBox->currentText().trimmed();
    if(user.isEmpty())
        return;
    QString password = ui->password_lineEdit->text().trimmed();
    if(password.isEmpty())
        return;


    SkpSocketManage::m_loginCode = user.toULongLong();

    Login::Login login;
    login.set_login_type(Login::LoginTypeCode);
    login.set_user(user.toULongLong());
    login.set_password(password.toStdString());

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalLoginLogin);
    m_signal->skp_append_messge(login.ByteSize(), &login);

    m_signal->emit_sigKeep([this](const QByteArray &bytes)
    {
        qDebug() << "check onLogin";

        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());


        if(tlvResponse.skp_status()) {
            emit sigLoginServer(LoginStatusError);
        } else {
            emit sigLoginServer(LoginStatusSuccess);
        }

    });

}

void SkpLoginMainWidget::onLoginServer(int loginStatus)
{
    qDebug() << "onLogin success";

    if(loginStatus == LoginStatusSuccess) {
        m_mainWindow = new SkpMainWindow();
        m_mainWindow->skp_show();
        m_loginWidget->hide();
        //QjtMessageBox::information(this, tr("提示"), tr("登录成功"));
    } else {
        QjtMessageBox::information(this, tr("提示"), tr("登录失败"));
    }
}
