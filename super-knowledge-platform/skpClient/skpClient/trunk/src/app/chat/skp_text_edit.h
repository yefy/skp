#ifndef SKP_TEXT_EDIT_H
#define SKP_TEXT_EDIT_H

#include <QTextEdit>
#include <set>

typedef struct SkpChat_s SkpChat_t;

class SkpTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit SkpTextEdit(QWidget *parent = 0);
    ~SkpTextEdit();
    void wheelEvent(QWheelEvent *e);
    void set(quint64 myCode, quint64 friendCode, QString friendName);
    void add(const QVector<SkpChat_t> &chatList, bool isAck);
    void appendChat(const SkpChat_t &chatData);
    QVector<SkpChat_t> m_chatDataList;
    int m_lastSize;
    quint64 m_myCode;
    quint64 m_friendCode;
    QString m_friendName;
    bool m_isDB;

    std::set<SkpChat_t> m_setList;

signals:
    void sigDB(qint64 messageIndex, qint32 count);
};

#endif // SKP_TEXT_EDIT_H
