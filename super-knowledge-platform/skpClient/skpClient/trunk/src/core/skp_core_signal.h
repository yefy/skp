#ifndef SKP_CORE_SIGNAL_H
#define SKP_CORE_SIGNAL_H

#include <QObject>
#include "skp_local_tlv.h"
#include <QMutex>

class SkpSignal : public QObject, public SkpLocalTLV
{
    Q_OBJECT
public:
    explicit SkpSignal();
    explicit SkpSignal(char *pBuffer, int64 nSize);
    explicit SkpSignal(char *pBuffer, int64 nSize, int tlvSize, int headSize);
    virtual ~SkpSignal();
    void init_signal();
    void emit_sigKeep(std::function<void(const QByteArray &)> func);
    void emit_sigMoment(std::function<void(const QByteArray &)> func);
    void emit_register_ack(quint64 classType, std::function<void(QByteArray &)> func);

private:
    quint64 m_clientMark;
    QMutex m_clientMarkMutex;

signals:
    void sigKeep(const QByteArray &data, std::function<void(QByteArray &)> func);
    void sigMoment(const QByteArray &data, std::function<void(QByteArray &)> func);
    void sigRegisterAck(quint64 classType, std::function<void(QByteArray &)> func);

public slots:

};

#endif // SKP_CORE_SIGNAL_H
