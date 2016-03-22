#ifndef SKP_SOCKET_TEST_WIDGET_H
#define SKP_SOCKET_TEST_WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QElapsedTimer>
#include <QTimer>
#include "skp_core_tlv.h"

namespace Ui {
class SkpSocketTestWidget;
}


class SkpSocketTestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SkpSocketTestWidget(QWidget *parent = 0);
    ~SkpSocketTestWidget();

private:
    Ui::SkpSocketTestWidget *ui;

private slots:
    void skp_on_timeout_start();
    void skp_on_timer_socket_once();
    void skp_on_timer_socket();
    void skp_on_quit(qint64 threadID, qint64 writeSize, qint64 readSize, qint64 sec);
    void skp_on_socket_quit(qint64 threadID,qint64 writeSize, qint64 readSize, qint64 sec);
    void skp_on_timeout_stop();
    void skp_on_simple_socket();
    void skp_on_simple_send();
    void skp_on_simple_recv();
    void skp_on_simple_close();

    void skp_on_socket_task_start();
    void skp_on_timer_task_start();
    void skp_on_socket_conf();
    void skp_on_print();
    void skp_on_connect();

    void skp_on_simple_protocol();
    void skp_on_simple_protocol_close();

signals:
    void skp_sig_create_socket();
private:
    void skp_on_static(qint64 threadID, qint64 writeSize, qint64 readSize, qint64 sec);

private:

        QTimer m_timer;
        QElapsedTimer elapsedTimer;
        QList<QThread *> m_threadList;

        qint64 m_writeSize;
        qint64 m_readSize;
        qint64 m_sec;
        qint64 m_readavgSize;
        qint64 m_writavgeSize;

        qint64 m_connectNumber;
        qint64 m_realConnect;
        qint64 m_closeNumber;
        qint64 m_errorConnect;
        qint64 m_max;
        bool m_isStart;

        QTimer m_taskTimer;
        int m_autoConnectNumber;
        int m_showPrintNumber;

        QTimer m_statisticsTimer;
};

class SkpTcpSocketTask : public QObject
{
    Q_OBJECT
public:
    SkpTcpSocketTask(QObject* parent = 0);
    ~SkpTcpSocketTask();

public slots:
    void skpOnCreateSocket();
    void skp_on_connect();
    void skp_on_quit(qint64, qint64, qint64, qint64);
    void skp_on_socket_quit(qint64, qint64, qint64, qint64);
signals:
    void skp_sig_connect();
    void skp_sig_quit(qint64 threadID,qint64 writeSize, qint64 readSize, qint64 sec);
    void skp_sig_socket_quit(qint64 threadID,qint64 writeSize, qint64 readSize, qint64 sec);
};


class SkpTcpSocketTest : public QTcpSocket
{
    Q_OBJECT
#define BUFFER_SIZE   1024
public:
    explicit SkpTcpSocketTest(QObject* parent = 0);
    qint64 m_readSize;
    qint64 m_writeSize;
    qint64 m_count;
    char m_writeBuffer[BUFFER_SIZE];
    char m_readBuffer[BUFFER_SIZE];
    qint64 m_readSleep;
    qint64 m_writeSleep;
    qint64 m_readSkip;

    QElapsedTimer elapsedTimer;
    bool m_isDelete;

private:
    bool check_exit();

signals:
    void skp_sig_connect();
    void skp_sig_quit(qint64 threadID,qint64 writeSize, qint64 readSize, qint64 sec);
    void skp_sig_socket_quit(qint64 threadID,qint64 writeSize, qint64 readSize, qint64 sec);

private slots:
    void skp_on_connect();
    void skp_on_read();
    void skp_on_disconnected();
    void skp_on_error(QAbstractSocket::SocketError serr);
private:
    void skp_write();
};



class SkpTcpSocketProtocol : public QTcpSocket
{
    Q_OBJECT
public:
    explicit SkpTcpSocketProtocol(QObject* parent = 0);
    int sendNumber;
    #define TEMP_BUFFER_SIZE 4096
    char g_buffer[TEMP_BUFFER_SIZE];
    int number;
    int recvNumber;
    QList<QByteArray> gList;
    QByteArray array;
    SkpTLV *m_tlv;
    SkpTLV *m_tlvRecv;
    SkpTLV *m_tlvRecvInfo;
public slots:
    void skp_on_simple_conn();
    void skp_on_simple_recv();
    void skp_on_simple_send();
    void skp_on_simple_error(QAbstractSocket::SocketError);
};

#endif // SKP_SOCKET_TEST_WIDGET_H
