#ifndef SKP_TCP_SOCKET_H
#define SKP_TCP_SOCKET_H

#include <QTcpSocket>
#include <QByteArray>

class SkpTcpSocket : public QTcpSocket
{
    Q_OBJECT

public:
    SkpTcpSocket();
    ~SkpTcpSocket();
    void setServerID(quint64 proxy)
    {
        m_proxy = proxy;
    }

    void setKeep(quint8 isKeep)
    {
        m_isKeep = isKeep;
    }

    void setSend(quint64 send)
    {
        m_send = send;
    }

    void setIP(QString &ip)
    {
        m_ip = ip;
    }

    void setPort(quint16 port)
    {
        m_port = port;
    }

    void addHead();
    void addData(const QByteArray &byte);
    bool connectToProxy();

signals:
    void sigResponse(const QByteArray &data);

public slots:
    void onConnected();
    void onError(QAbstractSocket::SocketError);
    void onDisconnected();
    void onRead();


public:
    QByteArray m_bytes;
    quint64 m_proxy;
    quint8 m_isKeep;
    quint64 m_send;
    bool m_isConn;
    QString m_ip;
    quint16 m_port;

};

#endif // SKP_TCP_SOCKET_H
