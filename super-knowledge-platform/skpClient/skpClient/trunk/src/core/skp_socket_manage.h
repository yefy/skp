#ifndef SKP_SOCKET_MANAGE_H
#define SKP_SOCKET_MANAGE_H

#include <QObject>
#include <QVector>
#include <QThread>
#include <QHash>

typedef enum {
    SocketTypeKeep = 0,
    SocketTypeKeepMoment,
}SocketType;

class SkpTcpSocket;

class SkpSocketManage : public QObject
{
    Q_OBJECT
public:
    explicit SkpSocketManage(SocketType type, QObject *parent = 0);
    static void init();
    static SkpSocketManage *instance(SocketType type);

signals:
    void sigResponse(const QByteArray &data);
public slots:
    void onRequest(const QByteArray &data);

private:
    void addSocketMonent(quint64 proxyID, SkpTcpSocket *socket);
    SkpTcpSocket *findSocketMonent(quint64 proxyID);

public:
    static QVector<SkpSocketManage *> m_socketManages;
    static QVector<QThread *> m_threads;

    static quint64 m_loginCode;
    static QString m_connectIP;
    static quint16 m_connectPort;

    SocketType m_type;
    QHash<qint64, SkpTcpSocket *> m_hashKeep;
    QHash<qint64, QVector<SkpTcpSocket *> *> m_hashMoment;
};

#endif // SKP_SOCKET_MANAGE_H
