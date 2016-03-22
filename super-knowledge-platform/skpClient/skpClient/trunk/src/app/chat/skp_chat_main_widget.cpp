#include "skp_chat_main_widget.h"
#include "ui_skp_chat_main_widget.h"
#include "skp_core_signal.h"
#include "memory_file.pb.h"
#include "ack.pb.h"
#include "message.pb.h"
#include "skp_socket_manage.h"
#include "skp_public_ins.h"
#include "skp_public_message_class.h"
#include "skp_core_manage.h"


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QFileDialog>


#define IS_INSERT_MORE 1
#include <QElapsedTimer>

SkpChatMainData::SkpChatMainData(QObject *parent) :
    QObject(parent),
    m_isRequest(false)
{
    m_signalMessage = new SkpSignal(NULL, 0);
}

SkpChatMainData::~SkpChatMainData()
{
}

void SkpChatMainData::setData(quint64 myCode, quint64 friendCode)
{
    m_myCode = myCode;
    m_friendCode = friendCode;
}

void SkpChatMainData::onDB(qint64 messageIndex, qint32 count)
{
    SkpChatRequest_t request;
    request.type = SkpChatRequestTypeCount;
    request.messageIndex = messageIndex;
    request.count = count;

    addRequest(request);
    startRequest();
}

void SkpChatMainData::onDB(const QVector<SkpChatIndex_t> &messageIndexList)
{
    SkpChatRequest_t request;
    request.type = SkpChatRequestTypeRange;
    request.messageIndexList = messageIndexList;

    addRequest(request);
    startRequest();

}

void SkpChatMainData::onAck(const QByteArray &ackStr)
{
    SkpChatRequest_t request;
    request.type = SkpChatRequestTypeAck;
    request.ackStr = ackStr;

    addRequest(request);
    startRequest();

}

void SkpChatMainData::onDB(const QVector<SkpChat_t> &chatList)
{
    replace_chat_more_db(chatList);
    emit sigDB(chatList, true);
}

qint64 SkpChatMainData::select_chat_max_db()
{
    QSqlQuery query;
    query.prepare("SELECT MessageIndex FROM `table_chat` \
                  WHERE UserChat = ? AND FriendChat = ? \
            ORDER BY MessageIndex DESC LIMIT 1");
    query.addBindValue(m_myCode);
    query.addBindValue(m_friendCode);
    query.exec();

    while(query.next()){
        return query.value(0).toULongLong();
    }

    return 0;
}


void SkpChatMainData::select_chat_more_db(qint64 messageIndex, qint32 count)
{
    QVector<SkpChatIndex_t> messageIndexList;

    for(int i = 0; i < count; i++) {
        qint64 tempIndex = messageIndex - i;
        if(tempIndex >= 1) {
            SkpChatIndex_t chatIndex;
            chatIndex.index = messageIndex - i;
            chatIndex.realIndex = 0;
            chatIndex.time = 0;
            messageIndexList.append(chatIndex);
        }
    }

    select_chat_more_db(messageIndexList);
}

void SkpChatMainData::select_chat_more_db(const QVector<SkpChatIndex_t> &messageIndexList)
{
    m_messageIndexList = messageIndexList;

    if(m_messageIndexList.isEmpty()) {
        QVector<SkpChat_t> chatList;
        emit sigDB(chatList, m_isAck);

        m_isRequest = false;
        startRequest();

        return;
    }



    QVector<SkpChatIndex_t> existMessageIndexList;
    is_exist_chat_more_db(m_messageIndexList, existMessageIndexList);

    QVector<SkpChatIndex_t> noExistMessageIndexList;
    for(int i = 0; i < m_messageIndexList.size(); i++) {
        const SkpChatIndex_t &chatIndex = m_messageIndexList.at(i);
        qint64 messageIndex = chatIndex.index;
        int j;
        for(j = 0; j < existMessageIndexList.size(); j++) {
            const SkpChatIndex_t &existChatIndex = existMessageIndexList.at(j);
            qint64 existMessageIndex = existChatIndex.index;
            if(messageIndex == existMessageIndex)
                break;
        }
        if(j == existMessageIndexList.size()) {
            noExistMessageIndexList.append(chatIndex);
        }
    }

    if(!noExistMessageIndexList.isEmpty())
        getUserData(m_myCode, m_friendCode, noExistMessageIndexList);
    else
        getFriendData(m_friendCode, m_myCode, m_messageIndexList);
}


void SkpChatMainData::select_chat_more_db(QVector<SkpChatIndex_t> &messageIndexList, QVector<SkpChat_t> &chatList)
{
    QElapsedTimer timer;
    timer.start();

    if(messageIndexList.isEmpty())
        return;

    QString querySql = "SELECT * FROM table_chat \
            WHERE UserChat = ? \
            AND FriendChat = ? \
            AND MessageIndex IN(";

    for(int i = 0; i < messageIndexList.size(); i++) {
        if(i != (messageIndexList.size() - 1)) {
            querySql += "?,";
        } else {
            querySql += "?)";
        }
    }

    QSqlQuery query;
    query.prepare(querySql);

    query.addBindValue(m_myCode);
    query.addBindValue(m_friendCode);

    for(int i = 0; i < messageIndexList.size(); i++) {
        const SkpChatIndex_t &messageIndex = messageIndexList.at(i);
        //qDebug() << "messageIndex" << messageIndex.index << messageIndex.realIndex;
        query.addBindValue(messageIndex.index);
    }

    query.exec();

    while(query.next()){
        SkpChat_t chatData;
        chatData.id = query.value(0).toULongLong();
        chatData.userChat = query.value(1).toULongLong();
        chatData.friendChat = query.value(2).toULongLong();
        chatData.index = query.value(3).toULongLong();
        chatData.realIndex = query.value(4).toULongLong();
        chatData.time = query.value(5).toULongLong();
        chatData.class_ = query.value(6).toInt();
        chatData.type = query.value(7).toInt();
        chatData.sender = query.value(8).toULongLong();
        chatData.receiver = query.value(9).toULongLong();
        chatData.friend_ = query.value(10).toULongLong();
        chatData.msaterIndex = query.value(11).toULongLong();
        chatData.data = query.value(12).toString();

        chatList.append(chatData);
    }
    qDebug() << "select_chat_more_db" << timer.elapsed() << messageIndexList.size() << chatList.size();
}


void SkpChatMainData::update_chat_index_more_db(QVector<SkpChatIndex_t> &messageIndexList)
{
    QElapsedTimer timer;
    timer.start();

    if(messageIndexList.isEmpty())
        return;

    QString querySql = "SELECT MessageIndex, MessageRealIndex, Time FROM table_chat \
            WHERE UserChat = ? \
            AND FriendChat = ? \
            AND MessageIndex IN(";

    for(int i = 0; i < messageIndexList.size(); i++) {
        if(i != (messageIndexList.size() - 1)) {
            querySql += "?,";
        } else {
            querySql += "?)";
        }
    }

    QSqlQuery query;
    query.prepare(querySql);

    query.addBindValue(m_myCode);
    query.addBindValue(m_friendCode);

    for(int i = 0; i < messageIndexList.size(); i++) {
        const SkpChatIndex_t &messageIndex = messageIndexList.at(i);
        query.addBindValue(messageIndex.index);
    }

    query.exec();

    messageIndexList.clear();
    while(query.next()){
        SkpChatIndex_t chatIndex;
        chatIndex.index = query.value(0).toULongLong();
        chatIndex.realIndex = query.value(1).toULongLong();
        chatIndex.time = query.value(2).toULongLong();

        messageIndexList.append(chatIndex);
    }

    qDebug() << "select_chat_more_db" << timer.elapsed();
}



void SkpChatMainData::is_exist_chat_more_db(QVector<SkpChatIndex_t> &messageIndexList, QVector<SkpChatIndex_t> &existMessageIndexList)
{
    QElapsedTimer timer;
    timer.start();

    if(messageIndexList.isEmpty())
        return;

    QString querySql = "SELECT MessageIndex FROM table_chat \
            WHERE UserChat = ? \
            AND FriendChat = ? \
            AND MessageIndex IN(";

    for(int i = 0; i < messageIndexList.size(); i++) {
        if(i != (messageIndexList.size() - 1)) {
            querySql += "?,";
        } else {
            querySql += "?)";
        }
    }

    QSqlQuery query;
    query.prepare(querySql);

    query.addBindValue(m_myCode);
    query.addBindValue(m_friendCode);

    for(int i = 0; i < messageIndexList.size(); i++) {
        const SkpChatIndex_t &chatIndex = messageIndexList.at(i);
        query.addBindValue(chatIndex.index);
    }

    query.exec();

    while(query.next()){
        qint64 messageIndex = query.value(0).toULongLong();

        SkpChatIndex_t chatIndex;
        chatIndex.index = messageIndex;

        existMessageIndexList.append(chatIndex);
    }
    qDebug() << "is_no_exist_chat_more_db" << timer.elapsed();
}

void SkpChatMainData::is_exist_chat_more_friend_db(QVector<SkpChatIndex_t> &messageIndexList, QVector<SkpChatIndex_t> &existMessageIndexList)
{
    QElapsedTimer timer;
    timer.start();

    if(messageIndexList.isEmpty())
        return;

    QString querySql = "SELECT MessageIndex, MessageRealIndex FROM table_chat \
            WHERE UserChat = ? \
            AND FriendChat = ? \
            AND Friend = ? \
            AND Data = '' \
            AND MessageIndex IN(";

    for(int i = 0; i < messageIndexList.size(); i++) {
        if(i != (messageIndexList.size() - 1)) {
            querySql += "?,";
        } else {
            querySql += "?)";
        }
    }

    QSqlQuery query;
    query.prepare(querySql);

    query.addBindValue(m_myCode);
    query.addBindValue(m_friendCode);
    query.addBindValue(m_friendCode);

    for(int i = 0; i < messageIndexList.size(); i++) {
        const SkpChatIndex_t &chatIndex = messageIndexList.at(i);
        //qDebug() << "chatIndex.inde" << chatIndex.index << "chatIndex.realIndex" << chatIndex.realIndex;

        query.addBindValue(chatIndex.index);
    }

    query.exec();

    while(query.next()){
        qint64 messageIndex = query.value(0).toULongLong();
        qint64 messageRealIndex = query.value(1).toULongLong();

        SkpChatIndex_t chatIndex;
        chatIndex.index = messageIndex;
        chatIndex.realIndex = messageRealIndex;

        //qDebug() << "friend chatIndex.inde" << chatIndex.index << "chatIndex.realIndex" << chatIndex.realIndex;


        existMessageIndexList.append(chatIndex);
    }

    qDebug() << "is_exist_chat_more_friend_db" << timer.elapsed();
}

void SkpChatMainData::replace_chat_more_db(const QVector<SkpChat_t> &chatList)
{
    if(chatList.isEmpty())
        return;

    QString querySql = "REPLACE INTO table_chat \
            (UserChat, FriendChat, MessageIndex, MessageRealIndex, Time, Class, Type, Sender, Receiver, Friend, MasterIndex, Data) \
            VALUES";
    for(int i = 0; i < chatList.size(); i++) {
        if(i != (chatList.size() - 1)) {
            querySql += "(?,?,?,?,?,?,?,?,?,?,?,?),";
        } else {
            querySql += "(?,?,?,?,?,?,?,?,?,?,?,?)";
        }
    }

    QSqlQuery query;
    query.prepare(querySql);

    for(int i = 0; i < chatList.size(); i++) {
        const SkpChat_t &chatData = chatList.at(i);

        query.addBindValue(chatData.userChat);
        query.addBindValue(chatData.friendChat);
        query.addBindValue(chatData.index);
        query.addBindValue(chatData.realIndex);
        query.addBindValue(chatData.time);
        query.addBindValue(chatData.class_);
        query.addBindValue(chatData.type);
        query.addBindValue(chatData.sender);
        query.addBindValue(chatData.receiver);
        query.addBindValue(chatData.friend_);
        query.addBindValue(chatData.msaterIndex);
        query.addBindValue(chatData.data);
    }

    query.exec();
}


void SkpChatMainData::addRequest(SkpChatRequest_t &request)
{
    m_requestList.append(request);
}

void SkpChatMainData::startRequest()
{
    if(m_isRequest)
        return;
    if(m_requestList.isEmpty())
        return;

    m_isRequest = true;

    SkpChatRequest_t request = m_requestList.takeAt(0);
    m_isAck = (request.type == SkpChatRequestTypeAck)? true:false;
    if(request.type == SkpChatRequestTypeCount) {
        if(request.messageIndex == 0) {
            request.messageIndex = select_chat_max_db();
        }

        if(request.messageIndex)
            select_chat_more_db(request.messageIndex, request.count);
        else
            getUserData(m_myCode, m_friendCode);

    } else if(request.type == SkpChatRequestTypeRange) {
        select_chat_more_db(request.messageIndexList);
    } else if(request.type == SkpChatRequestTypeAck) {
        getUserData(m_myCode, request.ackStr);
    }
}

void SkpChatMainData::getUserData(quint64 friendCode, quint64 keycode, QVector<SkpChatIndex_t> &messageIndexList)
{
    Message::MessageRequest request;
    request.set_class_(MessageClass_Chat);
    request.set_passage(MessagePassage_Chat);
    request.set_friend_(keycode);
    foreach (const SkpChatIndex_t &filterMessageIndex, messageIndexList) {

        request.add_index(filterMessageIndex.index);
    }

    getUserData_d(friendCode, request);
}


void SkpChatMainData::getUserData(quint64 friendCode, const QByteArray &ackStr)
{
    Ack::MessageAck ack;
    ack.ParseFromArray(ackStr.data(), ackStr.size());

    qint64 start = select_chat_max_db();


    m_messageIndexList.clear();

    Message::MessageRequest request;
    request.set_class_(ack.class_());
    request.set_passage(ack.passage());
    request.set_friend_(ack.friend_());
    Message::MessageRequestRange *range = request.add_index_range();
    range->set_start(start);
    range->set_end(0);
    range->set_length(10);


    getUserData_d(friendCode, request);
}

void SkpChatMainData::getUserData(quint64 friendCode, quint64 keycode)
{
    m_messageIndexList.clear();

    Message::MessageRequest request;
    request.set_class_(MessageClass_Chat);
    request.set_passage(MessagePassage_Chat);
    request.set_friend_(keycode);
    Message::MessageRequestRange *range = request.add_index_range();
    range->set_start(0);
    range->set_end(0);
    range->set_length(10);


    getUserData_d(friendCode, request);
}


void SkpChatMainData::getUserData_d(quint64 friendCode, Message::MessageRequest &request)
{
    m_signalMessage->skp_start();
    m_signalMessage->skp_set_ins(OrderLocalMessageGetMessage);
    m_signalMessage->skp_set_recv(friendCode);
    m_signalMessage->skp_append_messge(request.ByteSize(), &request);
    m_signalMessage->emit_sigKeep([this](const QByteArray &bytes)
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());
        Message::MessageDataList messageDataList;
        tlvResponse.skp_Parse_message(&messageDataList);

        QVector<SkpChat_t> chatDataList;

        bool isAck = m_messageIndexList.isEmpty()? true:false;

        for(int i = 0; i < messageDataList.data_size(); i++) {
            const Message::MessageData &messageData = messageDataList.data(i);
            SkpChat_t chatData;
            chatData.userChat = m_myCode;
            chatData.friendChat = m_friendCode;
            chatData.index = messageData.index();
            chatData.realIndex = messageData.real_index();
            chatData.time = messageData.time();
            chatData.class_ = messageData.class_();
            chatData.type = messageData.type();
            chatData.sender = messageData.sender();
            chatData.receiver = messageData.receiver();
            chatData.friend_ = messageData.friend_();
            chatData.msaterIndex = messageData.master_index();
            chatData.data = QString::fromStdString(messageData.data_1());

            chatDataList.append(chatData);


            if(isAck) {
                SkpChatIndex_t chatIndex;
                chatIndex.index = chatData.index;
                chatIndex.realIndex = chatData.realIndex;
                chatIndex.time = chatData.time;
                m_messageIndexList.append(chatIndex);
            }
        }

        replace_chat_more_db(chatDataList);


        getFriendData(m_friendCode, m_myCode, m_messageIndexList);

    });
}


void SkpChatMainData::getFriendData(quint64 friendCode, quint64 keycode, QVector<SkpChatIndex_t> &messageIndexList)
{
    update_chat_index_more_db(messageIndexList);

    QVector<SkpChatIndex_t> friendMessageIndexList;
    is_exist_chat_more_friend_db(messageIndexList, friendMessageIndexList);

    if(friendMessageIndexList.isEmpty()) {
        QVector<SkpChat_t> chatList;
        select_chat_more_db(m_messageIndexList, chatList);

        emit sigDB(chatList, m_isAck);

        m_isRequest = false;
        startRequest();

        return;
    }

    Message::MessageRequest request;
    request.set_class_(MessageClass_Chat);
    request.set_passage(MessagePassage_Chat);
    request.set_friend_(keycode);
    foreach (const SkpChatIndex_t &filterMessageIndex, friendMessageIndexList) {
        request.add_index(filterMessageIndex.realIndex);
    }


    m_signalMessage->skp_start();
    m_signalMessage->skp_set_ins(OrderLocalMessageGetMessage);
    m_signalMessage->skp_set_recv(friendCode);
    m_signalMessage->skp_append_messge(request.ByteSize(), &request);
    m_signalMessage->emit_sigKeep([this](const QByteArray &bytes)
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());
        Message::MessageDataList messageDataList;
        tlvResponse.skp_Parse_message(&messageDataList);

        QVector<SkpChat_t> chatDataList;

        for(int i = 0; i < messageDataList.data_size(); i++) {
            const Message::MessageData &messageData = messageDataList.data(i);
            SkpChat_t chatData;
            chatData.userChat = m_myCode;
            chatData.friendChat = m_friendCode;
            qint64 realIndex = messageData.index();
            const SkpChatIndex_t &chatIndex = getMessageIndex(realIndex);
            chatData.index = chatIndex.index;
            chatData.realIndex = chatIndex.realIndex;
            chatData.time = chatIndex.time;
            chatData.class_ = messageData.class_();
            chatData.type = messageData.type();
            chatData.sender = messageData.sender();
            chatData.receiver = messageData.receiver();
            chatData.friend_ = messageData.friend_();
            chatData.msaterIndex = messageData.master_index();
            chatData.data = QString::fromStdString(messageData.data_1());

            qDebug() << chatData.index << chatData.realIndex << chatData.time << chatData.data;

            chatDataList.append(chatData);
        }

        replace_chat_more_db(chatDataList);

        QVector<SkpChat_t> chatList;
        select_chat_more_db(m_messageIndexList, chatList);

        emit sigDB(chatList, m_isAck);

        m_isRequest = false;
        startRequest();
    });
}

SkpChatIndex_t SkpChatMainData::getMessageIndex(qint64 messageRealIndex)
{
    foreach (const SkpChatIndex_t &chatIndex, m_messageIndexList) {
        if(chatIndex.realIndex == messageRealIndex)
            return chatIndex;
    }

    SkpChatIndex_t chatIndex;
    memset(&chatIndex, 0x00, sizeof(chatIndex));

    return chatIndex;
}

//------------------------------------------------------------------
SkpChatMainWidget::SkpChatMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkpChatMainWidget)
{
    ui->setupUi(this);

    ui->transferFile_widget->setFixedWidth(250);
    ui->transferFile_widget->setHidden(true);
    ui->chating_textEdit->setFixedHeight(100);

    connect(ui->sender_pushButton, SIGNAL(clicked()), this, SLOT(onSender()));
    connect(this, SIGNAL(sigSenderOK(QVector<SkpChat_t>)), this, SLOT(onSenderOK(QVector<SkpChat_t>)));
    connect(ui->senderFile_pushButton, SIGNAL(clicked()), this, SLOT(onSenderFile()));
    connect(ui->chat_textEdit, SIGNAL(sigDB(qint64,qint32)), SIGNAL(sigDB(qint64,qint32)));

    m_signal = new SkpSignal(NULL, 0);

    m_chatMainData = new SkpChatMainData();
    QThread *thead = SkpManage::instanceThread();
    m_chatMainData->moveToThread(thead);
    qRegisterMetaType<QVector<SkpChat_t>>("QVector<SkpChat_t>");
    connect(this, SIGNAL(sigDB(qint64,qint32)), m_chatMainData, SLOT(onDB(qint64,qint32)));
    connect(this, SIGNAL(sigDB(QVector<SkpChat_t>)), m_chatMainData, SLOT(onDB(QVector<SkpChat_t>)));
    connect(this, SIGNAL(sigAck(QByteArray)), m_chatMainData, SLOT(onAck(QByteArray)));
    connect(m_chatMainData, SIGNAL(sigDB(QVector<SkpChat_t>,bool)), this, SLOT(onDB(QVector<SkpChat_t>,bool)));

    ///yefy___
    ui->chating_textEdit->append(tr("test"));
}

SkpChatMainWidget::~SkpChatMainWidget()
{
    delete ui;
}

void SkpChatMainWidget::setData(QString &friendCode, QString &friendName)
{
    m_myCode = SkpSocketManage::m_loginCode;
    m_friendCode = friendCode.toULongLong();
    m_friendName = friendName;
    QString name = m_friendName + tr("(") + friendCode + tr(")");
    ui->name_label->setText(name);

    ui->chat_textEdit->set(m_myCode, m_friendCode, m_friendName);

    m_chatMainData->setData(m_myCode, m_friendCode);

    emit sigDB(0, 10);
}

void SkpChatMainWidget::setAck(Ack::MessageAck &ack)
{
    int class_ = ack.class_();
    if(class_ == MessageClass_Chat) {
        const std::string &ackStr = ack.SerializeAsString();
        QByteArray array(ackStr.c_str(), ackStr.size());
        emit sigAck(array);
    } else if(class_ == MessageClass_MemoryFile) {
        ui->transferFile_widget->setAck(ack);
    }
}


void SkpChatMainWidget::onSender()
{
    ui->sender_pushButton->setEnabled(false);

    m_data = ui->chating_textEdit->toPlainText().trimmed();
    if(m_data.isEmpty())
        return;

    static int number = 0;
    number++;
    m_data = QString::number(number);

    SkpChat_t chatData;
    chatData.index = 100000000;
    chatData.friend_ = m_myCode;
    chatData.data = m_data;

    ui->chat_textEdit->appendChat(chatData);

    Message::MessageChat chat;
    chat.set_data(m_data.toStdString());
    chat.set_receiver(m_friendCode);

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalChat);
    m_signal->skp_append_messge(chat.ByteSize(), &chat);

    m_signal->emit_sigKeep([this](const QByteArray &bytes) mutable
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());

        Message::MessageData messageData;
        tlvResponse.skp_Parse_message(&messageData);

        QVector<SkpChat_t> chatDataList;

        SkpChat_t chatData;
        chatData.userChat = m_myCode;
        chatData.friendChat = m_friendCode;
        chatData.index = messageData.index();
        chatData.realIndex = messageData.real_index();
        chatData.time = messageData.time();
        chatData.class_ = messageData.class_();
        chatData.type = messageData.type();
        chatData.sender = messageData.sender();
        chatData.receiver = messageData.receiver();
        chatData.friend_ = messageData.friend_();
        chatData.msaterIndex = messageData.master_index();
        chatData.data = m_data;

        chatDataList.append(chatData);

        emit sigDB(chatDataList);

        emit sigSenderOK(chatDataList);
    });
}

void SkpChatMainWidget::onSenderOK(const QVector<SkpChat_t> &chatDataList)
{
    ui->chat_textEdit->add(chatDataList, true);
    ui->sender_pushButton->setEnabled(true);
}

void SkpChatMainWidget::onSenderFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("*.*"));
    if(fileName.isEmpty())
        return;

    ui->transferFile_widget->setPush(fileName, m_friendCode);
}

void SkpChatMainWidget::onDB(const QVector<SkpChat_t> &chatList, bool isAck)
{
    ui->chat_textEdit->add(chatList, isAck);
}
