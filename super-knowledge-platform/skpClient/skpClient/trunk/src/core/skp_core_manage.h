#ifndef SKP_CORE_MANAGE_H
#define SKP_CORE_MANAGE_H

#include <QObject>
#include <QHash>
#include <QMap>
#include <QByteArray>

class SkpTcpSocket;
class SkpSignal;

typedef struct SkpInsKey_s SkpInsKey_t;
struct SkpInsKey_s {
    quint64 ins;
    quint64 clientMark;
    bool operator < (const SkpInsKey_t &insKey) const
    {
        int res = memcmp(this, &insKey, sizeof(SkpInsKey_t));
        return (res < 0);
    }
//    bool operator == (const SkpInsKey_t &insKey) const
//    {
//        return (ins == insKey.ins && clientMark == insKey.clientMark);
//    }
};

/*
typedef struct SkpDataCallback_s SkpDataCallback_t;
struct SkpDataCallback_s {
    QByteArray privateData;
    std::function<void(const QByteArray &)> func;
};
*/

//#define PARSE_DATA_LEN sizeof(SkpInsCallback_t)


class SkpManage : public QObject
{
    Q_OBJECT
protected:
    explicit SkpManage(QObject *parent = 0);
public:
    static SkpManage *instance();
    static QThread * instanceThread();

public:
    static SkpManage *m_manage;
    static QThread *m_thread;
    SkpSignal *m_signal;

    std::map<SkpInsKey_t, std::function<void(QByteArray &)> > m_hash;
    std::map<SkpInsKey_t, std::function<void(QByteArray &)> > m_hashAck;
    //QMap<SkpInsKey_t, std::function<void(QByteArray &)> > m_hash;
    //QMap<SkpInsKey_t, std::function<void(QByteArray &)> > m_hashAck;
    void registerIns(quint64 ins, std::function<void(QByteArray &)> func);
    void registerCallbackAck(QByteArray &bytes);
    void registerCallbackRealTime(QByteArray &bytes);

signals:
    void sigKeep(const QByteArray &data);
    void sigMoment(const QByteArray &data);
public slots:
    void onKeep(const QByteArray &data, std::function<void(QByteArray &)> func);
    void onMoment(const QByteArray &data, std::function<void(QByteArray &)> func);
    void onResponse(const QByteArray &data);
    void onRegisterAck(quint64 classType, std::function<void(QByteArray &)> func);
};

#endif // SKP_CORE_MANAGE_H
