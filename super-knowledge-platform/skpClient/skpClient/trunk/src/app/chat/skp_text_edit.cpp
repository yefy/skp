#include "skp_text_edit.h"
#include "skp_chat_main_widget.h"
#include <QDebug>
#include <QWheelEvent>
#include <QScrollBar>
#include <QTextCursor>

SkpTextEdit::SkpTextEdit(QWidget *parent) :
    QTextEdit(parent),
    m_isDB(false)
{
}

SkpTextEdit::~SkpTextEdit()
{
}


void SkpTextEdit::set(quint64 myCode, quint64 friendCode, QString friendName)
{
    m_myCode = myCode;
    m_friendCode = friendCode;
    m_friendName = friendName;
}

void SkpTextEdit::wheelEvent(QWheelEvent *e)
{
    QTextEdit::wheelEvent(e);
    QScrollBar *bar = verticalScrollBar();
    if(bar->value() == bar->minimum()) {
        const SkpChat_t &chatData = *m_setList.begin();
        if(chatData.index <= 1)
            return;
        m_isDB = true;
        emit sigDB(chatData.index - 1, 10);
    }

}

void SkpTextEdit::add(const QVector<SkpChat_t> &chatList, bool isAck)
{
    int lastSize = chatList.size();
    QScrollBar *bar = verticalScrollBar();
    int value = bar->value();
    foreach (const SkpChat_t &chatData, chatList) {
        m_setList.insert(chatData);
    }

    clear();

    foreach (const SkpChat_t &chatData, m_setList) {
        appendChat(chatData);
    }


    if(!isAck) {
        int max = bar->maximum();
        int size = m_setList.size();
        if(size)
            bar->setValue(max / size * lastSize);
        m_isDB = false;
    } else {
        bar->setValue(value);
    }
}

void SkpTextEdit::appendChat(const SkpChat_t &chatData)
{
    QString dataShow;
    if(chatData.friend_ == m_friendCode) {
        dataShow = m_friendName + " : " + chatData.data + "  " + QString::number(chatData.index);
    } else {
        dataShow = QString(tr("我")) + QString(tr(" : ")) + chatData.data + "  " + QString::number(chatData.index);
    }

    append(dataShow);

    //ui->chat_textEdit->append("<img src=':/ble_live.png'>");
}
