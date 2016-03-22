#include "skp_socket_test_widget.h"
#include "ui_skp_socket_test_widget.h"
#include "skp_custom_dialog.h"
#include "skp_public_transfer_protocal.h"
#include "skp_public_ins.h"

#include <time.h>

#include <QThread>
#include <QDebug>
#include <QWidget>
#include <QTime>
#include <QMutex>

#define ONCE_NUMBER     3000;
#define COUNT_NUMBER    1000
#define IS_CHECK_DATA   0

#define THREAD_NUMBER   10


int g_ShowPrintNumber = 0;

//static const char *connectIP = "172.16.100.40";
//static const char *connectIP = "192.168.164.134";
//static const char *connectIP = "127.0.0.1";
static const char *connectIP = "192.168.164.141";


static const quint16 connectPort = 22221;
//static const quint16 connectPort = 33331;


#define HEAD_LEN sizeof(SkpProtocalHead)
#define SEND_ONCE_NUMBER 1
#define SIGNAL_SEND 1
#define IS_SEND_HEAD 1
#define CONN_NUMBER_MAX 1
int connNumber = 0;
int clostNumber = 0;
int allRecvNumber = 0;


void skp_sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

SkpSocketTestWidget::SkpSocketTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkpSocketTestWidget),
    m_writeSize(0),
    m_readSize(0),
    m_sec(0),
    m_readavgSize(0),
    m_writavgeSize(0),

    m_connectNumber(0),
    m_realConnect(0),
    m_closeNumber(0),
    m_errorConnect(0),
    m_max(0),
    m_isStart(false),
    m_autoConnectNumber(0),
    m_showPrintNumber(0)
{
    ui->setupUi(this);

    //qDebug() << "main ID" << (qint64)QThread::currentThreadId();
    m_autoConnectNumber = 100;
    m_showPrintNumber = 1000;
    g_ShowPrintNumber = m_showPrintNumber;
    ui->autoconnect_lineEdit->setText(QString::number(m_autoConnectNumber));
    ui->showPrint_lineEdit->setText(QString::number(m_showPrintNumber));

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(skp_on_timer_socket()));

    connect(ui->startTimer, SIGNAL(clicked()), this, SLOT(skp_on_timeout_start()));
    connect(ui->stopTimer, SIGNAL(clicked()), this, SLOT(skp_on_timeout_stop()));
    connect(ui->timerOnce, SIGNAL(clicked()), this, SLOT(skp_on_timer_socket_once()));

    connect(ui->connectSocket, SIGNAL(clicked()), this, SLOT(skp_on_simple_socket()));
    connect(ui->socketSend, SIGNAL(clicked()), this, SLOT(skp_on_simple_send()));
    connect(ui->socketRecv, SIGNAL(clicked()), this, SLOT(skp_on_simple_recv()));
    connect(ui->socketClose, SIGNAL(clicked()), this, SLOT(skp_on_simple_close()));

    connect(ui->protocolConn, SIGNAL(clicked()), this, SLOT(skp_on_simple_protocol()));
    connect(ui->protocolConnClose, SIGNAL(clicked()), this, SLOT(skp_on_simple_protocol_close()));

    connect(&m_statisticsTimer, SIGNAL(timeout()), this, SLOT(skp_on_print()));
    ///yefy___
    //m_statisticsTimer.start(5000);

    connect(&m_taskTimer, SIGNAL(timeout()), this, SLOT(skp_on_socket_task_start()));
    connect(ui->timerTaskStart, SIGNAL(clicked()), this, SLOT(skp_on_timer_task_start()));
    connect(ui->onceSocketTaskStart, SIGNAL(clicked()), this, SLOT(skp_on_socket_task_start()));
    connect(ui->ok_pushButton, SIGNAL(clicked()), this, SLOT(skp_on_socket_conf()));

    for(int i = 0; i < THREAD_NUMBER; i++) {
        QThread *thread = new QThread();
        m_threadList.push_back(thread);
        thread->start();
        while(!thread->isRunning()) {
            ;
        }

        SkpTcpSocketTask *tcpSocketTask = new SkpTcpSocketTask();
        tcpSocketTask->moveToThread(thread);
        connect(this, SIGNAL(skp_sig_create_socket()), tcpSocketTask, SLOT(skpOnCreateSocket()));
        connect(tcpSocketTask, SIGNAL(skp_sig_connect()), this, SLOT(skp_on_connect()));
        connect(tcpSocketTask, SIGNAL(skp_sig_quit(qint64, qint64, qint64, qint64)), this, SLOT(skp_on_quit(qint64, qint64, qint64, qint64)));
        connect(tcpSocketTask, SIGNAL(skp_sig_socket_quit(qint64, qint64, qint64, qint64)), this, SLOT(skp_on_socket_quit(qint64, qint64, qint64, qint64)));

    }
}

SkpSocketTestWidget::~SkpSocketTestWidget()
{
    delete ui;
    //    while(m_threadList.size() > 0) {
    //        QThread *thread = m_threadList.takeFirst();
    //        thread->deleteLater();
    //    }
    //    m_threadList.clear();
}

void SkpSocketTestWidget::skp_on_timeout_start()
{
    ui->startTimer->setEnabled(false);
    ui->stopTimer->setEnabled(true);
    m_timer.start(100);
}

void SkpSocketTestWidget::skp_on_timeout_stop()
{
    ui->stopTimer->setEnabled(false);
    ui->startTimer->setEnabled(true);
    m_timer.stop();
}

void SkpSocketTestWidget::skp_on_timer_socket_once()
{
    skp_on_timer_socket();
}

void SkpSocketTestWidget::skp_on_timer_socket()
{
    int threadIndex = m_connectNumber % THREAD_NUMBER;
    QThread *thread = m_threadList.at(threadIndex);
    m_connectNumber++;

    SkpTcpSocketTest *tcpSocketTest = new SkpTcpSocketTest();

    connect(tcpSocketTest, SIGNAL(connected()), tcpSocketTest, SLOT(skp_on_connect()));
    connect(tcpSocketTest, SIGNAL(readyRead()), tcpSocketTest, SLOT(skp_on_read()));
    connect(tcpSocketTest, SIGNAL(disconnected()), tcpSocketTest, SLOT(skp_on_disconnected()));
    connect(tcpSocketTest, SIGNAL(error(QAbstractSocket::SocketError)),
            tcpSocketTest, SLOT(skp_on_error(QAbstractSocket::SocketError)));

    connect(tcpSocketTest, SIGNAL(skp_sig_quit(qint64, qint64, qint64, qint64)), this, SLOT(skp_on_quit(qint64, qint64, qint64, qint64)));
    connect(tcpSocketTest, SIGNAL(skp_sig_socket_quit(qint64, qint64, qint64, qint64)), this, SLOT(skp_on_socket_quit(qint64, qint64, qint64, qint64)));

    tcpSocketTest->connectToHost(connectIP, connectPort);
    tcpSocketTest->waitForConnected();

    tcpSocketTest->moveToThread(thread);
}

void SkpSocketTestWidget::skp_on_quit(qint64 threadID, qint64 writeSize, qint64 readSize, qint64 sec)
{
    m_errorConnect++;
    //qDebug() << "skp_on_quit ID"<< threadID << "main ID" << (qint64)QThread::currentThreadId();
    skp_on_static(threadID, writeSize, readSize, sec);
}

void SkpSocketTestWidget::skp_on_socket_quit(qint64 threadID, qint64 writeSize, qint64 readSize, qint64 sec)
{
    //qDebug() << "skp_on_socket_quit ID"<< threadID << "main ID" << (qint64)QThread::currentThreadId();
    skp_on_static(threadID, writeSize, readSize, sec);
}

void SkpSocketTestWidget::skp_on_static(qint64 threadID, qint64 writeSize, qint64 readSize, qint64 sec)
{
    m_writeSize += writeSize;
    m_readSize += readSize;
    m_sec += sec;

    m_readavgSize += readSize;
    m_writavgeSize += writeSize;
    m_closeNumber += 1;


    if(m_autoConnectNumber > 0 && (m_max - m_closeNumber) < m_autoConnectNumber) {
        if(!m_isStart) {
            skp_on_timer_task_start();
        }
    }

    /*
    qDebug() << "当前链接 "<<threadID << " writeSize = " << writeSize << " readSize = " << readSize <<" sec = "<< sec;
    qDebug() << "当前链接每秒" <<threadID<< " writeSize = " << writeSize / sec << " readSize = " << readSize/sec << " sec = " << sec;

    qDebug() << "总数据包"<< " m_writeSize = " << m_writeSize << " m_readSize = " << m_readSize;

    qDebug() << "总数据包平均每秒" << "m_writeSize = " << m_writeSize / m_sec << " m_readSize = " << m_readSize/m_sec << " m_sec = " << m_sec;

    qDebug() << "总数据包平均每个连接数据包" << "m_writeSize = " << m_writeSize / m_closeNumber << " m_readSize = " << m_readSize/m_closeNumber << " m_closeNumber = " << m_closeNumber;

    qint64 allsec = elapsedTimer.elapsed() / 1000;
    if(!allsec)
        allsec = 1;
    qDebug() << "总数据包每秒" << "m_writeSize = " << m_writeSize / allsec << " m_readSize = " << m_readSize/allsec << " m_sec = " << allsec;
    qDebug() << m_connectNumber << m_closeNumber << m_errorConnect << allsec;
    //qDebug() << m_connectNumber / allsec << m_closeNumber / allsec << m_errorConnect / allsec;
    qDebug() << "";
    qDebug() << "";
    qDebug() << "";
    */
}





















SkpTcpSocketTask::SkpTcpSocketTask(QObject* parent) :
    QObject(parent)
{
}

SkpTcpSocketTask::~SkpTcpSocketTask()
{
}

void SkpTcpSocketTask::skpOnCreateSocket()
{
    SkpTcpSocketTest *tcpSocketTest = new SkpTcpSocketTest();

    connect(tcpSocketTest, SIGNAL(connected()), tcpSocketTest, SLOT(skp_on_connect()));
    connect(tcpSocketTest, SIGNAL(readyRead()), tcpSocketTest, SLOT(skp_on_read()));
    connect(tcpSocketTest, SIGNAL(disconnected()), tcpSocketTest, SLOT(skp_on_disconnected()));
    connect(tcpSocketTest, SIGNAL(error(QAbstractSocket::SocketError)),
            tcpSocketTest, SLOT(skp_on_error(QAbstractSocket::SocketError)));

    connect(tcpSocketTest, SIGNAL(skp_sig_connect()), this, SLOT(skp_on_connect()));
    connect(tcpSocketTest, SIGNAL(skp_sig_quit(qint64, qint64, qint64, qint64)), this, SLOT(skp_on_quit(qint64, qint64, qint64, qint64)));
    connect(tcpSocketTest, SIGNAL(skp_sig_socket_quit(qint64, qint64, qint64, qint64)), this, SLOT(skp_on_socket_quit(qint64, qint64, qint64, qint64)));

    tcpSocketTest->connectToHost(connectIP, connectPort);
    //tcpSocketTest->waitForConnected();
}

void SkpTcpSocketTask::skp_on_connect()
{
    emit skp_sig_connect();
}

void SkpTcpSocketTask::skp_on_quit(qint64 threadID, qint64 writeSize, qint64 readSize, qint64 sec)
{
    emit skp_sig_quit(threadID, writeSize, readSize, sec);
}


void SkpTcpSocketTask::skp_on_socket_quit(qint64 threadID, qint64 writeSize, qint64 readSize, qint64 sec)
{
    emit skp_sig_socket_quit(threadID, writeSize, readSize, sec);
}






















SkpTcpSocketTest::SkpTcpSocketTest(QObject* parent) :
    QTcpSocket(parent),
    m_readSize(0),
    m_writeSize(0),
    m_count(0),
    m_isDelete(false),
    m_readSleep(0),
    m_writeSleep(0),
    m_readSkip(0)
{
    elapsedTimer.start();
}

bool SkpTcpSocketTest::check_exit()
{
    if(m_count >= COUNT_NUMBER) {
        skp_on_disconnected();
        return true;
    }
    return false;
}

void SkpTcpSocketTest::skp_on_connect()
{
    emit skp_sig_connect();
    skp_write();
}

void SkpTcpSocketTest::skp_write()
{
    if(check_exit())
        return;

    static int i = 0;
    if(i == 0) {
        i = 1;
        QTime time;
        time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
    }

    int max = 100;
    int min = 65;
    int data=(qrand()%(max - min + 1)) + min;

    memset(m_writeBuffer, data, BUFFER_SIZE);

    qint64 skip = 0;
    qint64 size = 0;
    qint64 left = BUFFER_SIZE;

    while(left > 0) {
        size = write(m_writeBuffer + skip, left);
        //qDebug() << "write size = " << size;
        if(size == 0) {
            //skp_sleep(2);
            QThread::msleep(5);
            m_writeSleep++;
            if(m_writeSleep % 10 == 0) {
                qDebug() << "write error ";
            }
            if(m_writeSleep == 50) {
                qDebug() << "write error len = 0";
                skp_on_error(QAbstractSocket::RemoteHostClosedError);
                return;
            }
        }

        if(size < 0) {
            qDebug() << "write error len < 0";
            skp_on_error(QAbstractSocket::RemoteHostClosedError);
            return;
        }

        if(m_isDelete)
            return;

        left -= size;
        skip += size;
        if(!IS_CHECK_DATA)
            break;
    }

    if(IS_CHECK_DATA)
        m_writeSize += BUFFER_SIZE;
    else
        m_writeSize += size;

    m_count++;


    if(g_ShowPrintNumber > 0 && m_count % g_ShowPrintNumber == 0)
        qDebug() << "m_count" << m_count;
}

void SkpTcpSocketTest::skp_on_read()
{
    if(check_exit())
        return;

    qint64 size = 0;
    qint64 left = BUFFER_SIZE - m_readSkip;

    if(left > 0) {
        size = read(m_readBuffer + m_readSkip, left);
        if(size <= 0) {
            return;
        }

        m_readSkip += size;

        if(m_readSkip < BUFFER_SIZE) {
            return;
        }

        if(m_readSkip == BUFFER_SIZE) {
            m_readSkip = 0;
        }
    } else {
        qDebug() << "read left = error" << left;
        QApplication::exit();
    }


    if(IS_CHECK_DATA) {
        if(memcmp(m_writeBuffer, m_readBuffer, BUFFER_SIZE) != 0) {
            qDebug() << "m_writeBuffer = " << m_writeBuffer;
            qDebug() << "m_readBuffer = " << m_readBuffer;
            qDebug() << "m_writeBuffer != m_readBuffer";
            QApplication::exit();
        }
    }

    if(IS_CHECK_DATA)
        m_readSize += BUFFER_SIZE;
    else
        m_readSize += size;

    skp_write();
}

void SkpTcpSocketTest::skp_on_disconnected()
{
    if(m_isDelete)
        return;
    m_isDelete = !m_isDelete;

    qint64 sec = elapsedTimer.elapsed() / 1000;
    sec = (sec > 0)?sec:1;

    emit skp_sig_socket_quit((qint64)QThread::currentThreadId(),m_writeSize, m_readSize, sec);
    deleteLater();
}

void SkpTcpSocketTest::skp_on_error(QAbstractSocket::SocketError err)
{
    qDebug() << "skp_on_error" << err <<"count" << m_count;

    if(m_isDelete)
        return;
    m_isDelete = !m_isDelete;
    qint64 sec = elapsedTimer.elapsed() / 1000;
    sec = (sec > 0)?sec:1;

    emit skp_sig_quit((qint64)QThread::currentThreadId(),m_writeSize, m_readSize, sec);
    deleteLater();
}





QElapsedTimer timer;
SkpTcpSocketProtocol::SkpTcpSocketProtocol(QObject* parent) :
    QTcpSocket(parent),
    sendNumber(0),
    number(65),
    recvNumber(0)
{
    m_tlv = new SkpTLV();
    m_tlvRecv = new SkpTLV();
    m_tlvRecvInfo = new SkpTLV();
}

void SkpTcpSocketProtocol::skp_on_simple_send()
{
    for(int i = 0; i < SEND_ONCE_NUMBER; i++) {
        quint64 send = 1;
        send = (send << 32) + 33331;
        quint64 recv =send;

        memset(g_buffer, number, TEMP_BUFFER_SIZE);
        number++;
        if(number > 65 + SEND_ONCE_NUMBER)
            number = 65;

        SkpProtocalHead *head = (SkpProtocalHead *)g_buffer;
        memset(head, 0x00, sizeof(SkpProtocalHead));
        SkpPakegeHead(head, 15, send, recv, OrderTypeRequest, 22221, OrderLocalTestNetwork, 0);

        m_tlv->reset();
        m_tlv->set_Head(head->headSize);
        memcpy(m_tlv->Head(), head, head->headSize);
        m_tlv->add(_Tag_System_Test, (char *)(g_buffer + head->headSize), head->dataSize);
        //qDebug() << "send*************************************";
        //qDebug() << head->send << head->recv << head->dataSize << head->headSize;
        QByteArray arryData(g_buffer + head->headSize, head->dataSize);
        //qDebug() << arryData.size() << arryData.data();
        head = (SkpProtocalHead *)m_tlv->Head();
        head->dataSize = m_tlv->tlv_size();

        gList.append(arryData);
        int allLen = m_tlv->use_size();
        //qDebug() << "allLen = " << allLen;
        int realSize = 0;

        while(realSize < allLen) {
            int size = write((char *)m_tlv->byte() + realSize, allLen - realSize);
            //qDebug() << "size = " << size;
            realSize += size;
        }


    }

    sendNumber = SEND_ONCE_NUMBER;
}

void SkpTcpSocketProtocol::skp_on_simple_conn()
{
    if(IS_SEND_HEAD) {
        quint64 send = 1;
        send = (send << 32) + 33331;

        SkpProtocalConnHead head;
        SkpPakegeConnHead(&head, 1, ConnTypeClient, 0, send, 22221);

        write((char *)&head, sizeof(SkpProtocalConnHead));
    }


    skp_on_simple_send();
}

void SkpTcpSocketProtocol::skp_on_simple_recv()
{
    do {
        QByteArray arr = readAll();
        array.append(arr);
        if(!SkpCheckHead(array.data(), array.size())) {
            //qDebug() << "SkpCheckHead return";
            return;
        }

        SkpProtocalHead *ask = (SkpProtocalHead *)array.data();
        m_tlvRecv->reset();
        m_tlvRecv->copy(array.data(), ask->dataSize, ask->headSize);
        int dataSize = m_tlvRecv->seek(_Tag_System_Test);
        //qDebug() << "_Tag_System_Test" << dataSize;
        char *data = m_tlvRecv->seek_value();

        //qDebug() << "recv++++++++++++++++++++++++++++";
        //qDebug() << ask->send << ask->recv << ask->dataSize << ask->headSize;
        array.remove(0, ask->headSize + ask->dataSize);
        QByteArray arryData(data, dataSize);

        //qDebug() << arryData.size() << arryData.data();
        int i;
        bool isBreak = false;
        for(i = 0; i < gList.size(); i++) {
            QByteArray arryDataSend = gList.at(i);
            //qDebug() << arryDataSend.size() << arryDataSend.data();
            if(arryData == arryDataSend) {
                gList.removeAt(i);
                isBreak = true;
                break;
            }
        }

        if(!isBreak) {
            qDebug() << "error";
            QApplication::exit(1);
            return;
        }


//        if((dataSize = m_tlvRecv->seek(_Tag_Data)))
//        {
//            do
//            {
//                m_tlvRecvInfo->reset_TLV();
//                m_tlvRecvInfo->copy(m_tlvRecv->seek_value(), dataSize, 0);
//                int userID = m_tlvRecvInfo->seek_int(_Tag_UserID);
//                int serverID = m_tlvRecvInfo->seek_int(_Tag_ServerID);
//                int password = m_tlvRecvInfo->seek(_Tag_Password);
//                char *value = m_tlvRecvInfo->seek_value();
//                //QByteArray arryData(value, password);
//                //qDebug() << userID << serverID << arryData.data() << password;
//                //qDebug() << "_Tag_Data" << dataSize;
//            } while(m_tlvRecv->next());
//        }


        //            QByteArray arryDataSend = gList.takeFirst();
        //            if(arryData != arryDataSend) {
        //                qDebug() << "arryData != arryDataSend";
        //                qDebug() << arryDataSend.size() << arryDataSend.data();
        //                QApplication::exit(1);
        //                return;
        //            }


        sendNumber--;
        recvNumber++;
        allRecvNumber++;
        if(allRecvNumber % 1000 == 0)
        {
            qDebug() << "sendNumber = " << sendNumber << "connNumber = " << connNumber << "closeNumber = " << clostNumber << (uint)this << "recvNumber" << recvNumber << "allRecvNumber" << allRecvNumber;
            qint64 time = timer.elapsed();
            time = time / 1000;
            if(time == 0)
                time = 1;
            qDebug() << "allRecvNumber " << allRecvNumber << "time" << time << "sec" << allRecvNumber / time;
        }
    }while(array.size() > 0);

    if(sendNumber == 0) {
        if(SIGNAL_SEND)
            skp_on_simple_send();
    }

    if(sendNumber < 0){
        qDebug() << "send number error";
        QApplication::exit(1);
        return;
    }

}

void SkpTcpSocketProtocol::skp_on_simple_error(QAbstractSocket::SocketError err)
{
    clostNumber++;
    qDebug() << "err = " << err << "connNumber = " << connNumber << "closeNumber = " << clostNumber << (uint)this << "recvNumber" << recvNumber << "allRecvNumber" << allRecvNumber;
}



QList<SkpTcpSocketProtocol *> protocolList;
void SkpSocketTestWidget::skp_on_simple_protocol()
{
    timer.start();
    for(int i = 0; i < CONN_NUMBER_MAX; i++) {
        SkpTcpSocketProtocol *m_tcpSocketTest = new SkpTcpSocketProtocol();
        m_tcpSocketTest->connectToHost(connectIP, connectPort);
        m_tcpSocketTest->waitForConnected();
        m_tcpSocketTest->skp_on_simple_conn();
        //connect(m_tcpSocketTest, SIGNAL(readyRead()), this, SLOT(skp_on_simple_recv()));
        //connect(m_tcpSocketTest, SIGNAL(connected()), m_tcpSocketTest, SLOT(skp_on_simple_conn()));

        int threadIndex = m_connectNumber % THREAD_NUMBER;
        QThread *thread = m_threadList.at(threadIndex);
        m_connectNumber++;
        m_tcpSocketTest->moveToThread(thread);

        protocolList.append(m_tcpSocketTest);

        connect(m_tcpSocketTest, SIGNAL(readyRead()), m_tcpSocketTest, SLOT(skp_on_simple_recv()));
        connect(m_tcpSocketTest, SIGNAL(error(QAbstractSocket::SocketError)), m_tcpSocketTest, SLOT(skp_on_simple_error(QAbstractSocket::SocketError)));
        connNumber++;
        qDebug() << "************* i = " << i << (uint)this;
    }
}

void SkpSocketTestWidget::skp_on_simple_protocol_close()
{
    for(int i = 0; i < protocolList.size(); i++)
    {
        SkpTcpSocketProtocol *tcpSocketTest = protocolList.at(i);
        tcpSocketTest->deleteLater();
    }

    protocolList.clear();
}





static char g_buffer[TEMP_BUFFER_SIZE];
SkpTcpSocketProtocol *m_tcpSocketTest = NULL;
static int number = 65;
QList<QByteArray> gList;
QByteArray array;
void SkpSocketTestWidget::skp_on_simple_socket()
{
    m_tcpSocketTest = new SkpTcpSocketProtocol();
    m_tcpSocketTest->connectToHost(connectIP, connectPort);
    m_tcpSocketTest->waitForConnected();
    //connect(m_tcpSocketTest, SIGNAL(readyRead()), this, SLOT(skp_on_simple_recv()));

    connect(m_tcpSocketTest, SIGNAL(readyRead()), this, SLOT(skp_on_simple_recv()));

    if(IS_SEND_HEAD) {
        quint64 send = 1;
        send = (send << 32) + 33331;

        SkpProtocalConnHead head;
        SkpPakegeConnHead(&head, 1, ConnTypeClient, 0, send, 22221);

        m_tcpSocketTest->write((char *)&head, sizeof(SkpProtocalConnHead));
    }

    //delete m_tcpSocketTest;
    //m_tcpSocketTest = NULL;
}
int sendNumber = 0;
void SkpSocketTestWidget::skp_on_simple_send()
{
    for(int i = 0; i < SEND_ONCE_NUMBER; i++) {
        quint64 send = 1;
        send = (send << 32) + 33331;
        quint64 recv =send;
        if(m_tcpSocketTest) {
            memset(g_buffer, number, TEMP_BUFFER_SIZE);
            number++;
            if(number > 65 + SEND_ONCE_NUMBER)
                number = 65;

            SkpProtocalHead *head = (SkpProtocalHead *)g_buffer;

            SkpPakegeHead(head, 15, send, recv, OrderTypeRequest, 22221, OrderLocalTestNetwork, 0);
            qDebug() << head->orderRequst_t.module;
            qDebug() << "send*************************************";
            qDebug() << head->send << head->recv << head->dataSize << head->headSize;
            QByteArray arryData(g_buffer + head->headSize, head->dataSize);
            qDebug() << arryData.size() << arryData.data();

            gList.append(arryData);
            int allLen = head->headSize + head->dataSize;
            qDebug() << "allLen = " << allLen;
            int realSize = 0;

            while(realSize < allLen) {
                int size = m_tcpSocketTest->write(g_buffer + realSize, allLen - realSize);
                qDebug() << "size = " << size;
                realSize += size;
            }
        }

    }

    sendNumber = SEND_ONCE_NUMBER;
}

void SkpSocketTestWidget::skp_on_simple_recv()
{
    if(m_tcpSocketTest)  {
        do {
            QByteArray arr = m_tcpSocketTest->readAll();
            array.append(arr);
            if(!SkpCheckHead(array.data(), array.size())) {
                qDebug() << "SkpCheckHead return";
                return;
            }

            SkpProtocalHead *ask = (SkpProtocalHead *)array.data();

            int dataLen = ask->dataSize;
            qDebug() << "recv++++++++++++++++++++++++++++";
            qDebug() << ask->send << ask->recv << ask->dataSize << ask->headSize;
            array.remove(0, HEAD_LEN);
            QByteArray arryData = array.mid(0, dataLen);

            qDebug() << arryData.size() << arryData.data();
            int i;
            bool isBreak = false;
            for(i = 0; i < gList.size(); i++) {
                QByteArray arryDataSend = gList.at(i);
                qDebug() << arryDataSend.size() << arryDataSend.data();
                if(arryData == arryDataSend) {
                    gList.removeAt(i);
                    isBreak = true;
                    break;
                }
            }

            if(!isBreak) {
                qDebug() << "error";
                QApplication::exit(1);
                return;
            }


            //            QByteArray arryDataSend = gList.takeFirst();
            //            if(arryData != arryDataSend) {
            //                qDebug() << "arryData != arryDataSend";
            //                qDebug() << arryDataSend.size() << arryDataSend.data();
            //                QApplication::exit(1);
            //                return;
            //            }


            array.remove(0, dataLen);
            sendNumber--;
            qDebug() << "sendNumber" << sendNumber;
        }while(array.size() > 0);

        if(sendNumber == 0) {
            if(SIGNAL_SEND)
                skp_on_simple_send();
        }

        if(sendNumber < 0){
            qDebug() << "send number error";
            QApplication::exit(1);
            return;
        }

    }
}

void SkpSocketTestWidget::skp_on_simple_close()
{
    if(m_tcpSocketTest)  {
        delete m_tcpSocketTest;
        m_tcpSocketTest = NULL;
    }
}



qint64 xxxxx = 0;
void SkpSocketTestWidget::skp_on_socket_task_start()
{
    static quint32 startTime = time(NULL);
    //static int emitNumber = 0;
    if(xxxxx % THREAD_NUMBER != 0) {
        quint32 lastTime = time(NULL);
        if(lastTime - startTime < 2) {
            //emitNumber++;
            //if(emitNumber < 400) {
            return;
        } else {
            xxxxx = (xxxxx / THREAD_NUMBER) * THREAD_NUMBER;
        }

    }

    //emitNumber = 0;
    startTime = time(NULL);

    m_connectNumber += THREAD_NUMBER;
    emit skp_sig_create_socket();

    if(m_connectNumber >= m_max) {
        if(m_isStart) {
            skp_on_timer_task_start();
        }
    }
}

void SkpSocketTestWidget::skp_on_timer_task_start()
{
    static bool state = false;
    if(!state) {
        m_isStart = true;
        m_max += ONCE_NUMBER;
        ui->timerTaskStart->setText("timerTaskStop");
        m_taskTimer.start(100);
    } else {
        m_isStart = false;
        ui->timerTaskStart->setText("timerTaskStart");
        m_taskTimer.stop();
    }

    state = !state;
}

void SkpSocketTestWidget::skp_on_socket_conf()
{
    m_autoConnectNumber = ui->autoconnect_lineEdit->text().toInt();
    m_showPrintNumber = ui->showPrint_lineEdit->text().toInt();
    g_ShowPrintNumber = m_showPrintNumber;
}

void SkpSocketTestWidget::skp_on_print()
{
    qDebug() << "connect" << m_connectNumber << "realConnect" << m_realConnect << "max" << m_max << "close" << m_closeNumber << "error" << m_errorConnect;
}

void SkpSocketTestWidget::skp_on_connect()
{
    xxxxx++;
    m_realConnect++;
    //qDebug() << "m_realConnect" << m_realConnect;
}
