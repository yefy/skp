#ifndef SKP_CHAT_MAIN_WIDGET_H
#define SKP_CHAT_MAIN_WIDGET_H

#include <QWidget>
#include <QMutex>

class SkpSignal;

namespace Ui {
class SkpChatMainWidget;
}

namespace Message {
    class MessageDataList;
    class MessageData;
    class MessageRequest;
}

namespace Ack {
    class MessageAck;
}

typedef struct SkpChat_s SkpChat_t;
struct SkpChat_s
{
    quint64 id;
    quint64 userChat;
    quint64 friendChat;
    quint64 index;
    quint64 realIndex;
    quint64 time;
    qint32 class_;
    qint32 type;
    quint64 sender;
    quint64 receiver;
    quint64 friend_;
    quint64 msaterIndex;
    QString data;

    bool operator < (const SkpChat_t &rhs) const//升序排序时必须写的函数
    {
        return index < rhs.index;
    }

//    bool operator == (const SkpChat_t &rhs) const//降序排序时必须写的函数
//    {
//        return index == rhs.index;
//    }

//    bool operator > (const SkpChat_t &rhs) const//降序排序时必须写的函数
//    {
//        return index > rhs.index;
//    }

//    bool operator() (const SkpChat_t &rhs_1, const SkpChat_t &rhs_2) const
//    {
//        return rhs_1.index < rhs_2.index;
//    }
};

//inline uint qHash(const SkpChat_t &chat, uint seed = 0) {
//    return qHash(chat.index, seed^0xa03f);
//}

typedef struct SkpChatIndex_s SkpChatIndex_t;
struct SkpChatIndex_s
{
    qint64 index;
    qint64 realIndex;
    qint64 time;
};

typedef enum {
    SkpChatRequestTypeCount = 0,
    SkpChatRequestTypeRange,
    SkpChatRequestTypeAck,
}SkpChatRequestType;

typedef struct SkpChatRequest_s SkpChatRequest_t;
struct SkpChatRequest_s
{
    qint32 type;
    qint64 messageIndex;
    qint32 count;
    QVector<SkpChatIndex_t> messageIndexList;
    QByteArray ackStr;
};

class SkpChatMainData : public QObject
{
    Q_OBJECT
public:
    explicit SkpChatMainData(QObject *parent = 0);
    ~SkpChatMainData();
    void setData(quint64 myCode, quint64 friendCode);
private:
    quint64 m_myCode;
    quint64 m_friendCode;
    SkpSignal *m_signalMessage;
    QVector<SkpChatIndex_t> m_messageIndexList;
    QList<SkpChatRequest_t> m_requestList;
    bool m_isRequest;
    bool m_isAck;
private:
    qint64 select_chat_max_db();
    void select_chat_more_db(qint64 messageIndex, qint32 count);
    void select_chat_more_db(const QVector<SkpChatIndex_t> &messageIndexList);

    void select_chat_more_db(QVector<SkpChatIndex_t> &messageIndexList, QVector<SkpChat_t> &chatList);
    void update_chat_index_more_db(QVector<SkpChatIndex_t> &messageIndexList);
    void is_exist_chat_more_db(QVector<SkpChatIndex_t> &messageIndexList, QVector<SkpChatIndex_t> &existMessageIndexList);
    void is_exist_chat_more_friend_db(QVector<SkpChatIndex_t> &messageIndexList, QVector<SkpChatIndex_t> &existMessageIndexList);
    void replace_chat_more_db(const QVector<SkpChat_t> &chatList);

    void addRequest(SkpChatRequest_t &request);
    void startRequest();

    void getUserData(quint64 friendCode, quint64 keycode, QVector<SkpChatIndex_t> &messageIndexList);
    void getUserData(quint64 friendCode, const QByteArray &ackStr);
    void getUserData(quint64 friendCode, quint64 keycode);
    void getUserData_d(quint64 friendCode, Message::MessageRequest &request);
    void getFriendData(quint64 friendCode, quint64 keycode, QVector<SkpChatIndex_t> &messageIndexList);
    SkpChatIndex_t getMessageIndex(qint64 messageRealIndex);

public slots:
    void onDB(qint64 messageIndex, qint32 count);
    void onDB(const QVector<SkpChatIndex_t> &messageIndexList);
    void onAck(const QByteArray &ackStr);

    void onDB(const QVector<SkpChat_t> &chatList);
signals:
    void sigDB(const QVector<SkpChat_t> &chatList, bool isAck);
};

class SkpChatMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SkpChatMainWidget(QWidget *parent = 0);
    ~SkpChatMainWidget();
    void setData(QString &friendCode, QString &friendName);
    void setAck(Ack::MessageAck &ack);

private:
    Ui::SkpChatMainWidget *ui;
    SkpChatMainData *m_chatMainData;
    quint64 m_myCode;
    quint64 m_friendCode;
    QString m_friendName;
    SkpSignal *m_signal;
    QString m_data;
    QVector<SkpChat_t> m_chatList;

signals:
    void sigDB(qint64 messageIndex, qint32 count);
    void sigAck(const QByteArray &ackStr);
    void sigDB(const QVector<SkpChat_t> &chatList);

public slots:
    void onDB(const QVector<SkpChat_t> &chatList, bool isAck);

signals:
    void sigSenderOK(const QVector<SkpChat_t> &chatDataList);
private slots:
    void onSender();
    void onSenderOK(const QVector<SkpChat_t> &chatDataList);
    void onSenderFile();
};

#endif // SKP_CHAT_MAIN_WIDGET_H
