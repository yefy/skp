#include "skp_transfer_file_widget.h"
#include "ui_skp_transfer_file_widget.h"
#include "skp_transfer_file_item_widget.h"
#include "message.pb.h"
#include "ack.pb.h"
#include "memory_file.pb.h"

#include "skp_local_tlv.h"
#include "skp_public_message_class.h"
#include "skp_core_signal.h"
#include "skp_socket_manage.h"
#include "skp_public_ins.h"

#include <QDebug>

SkpTransferFileWidget::SkpTransferFileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkpTransferFileWidget)
{
    ui->setupUi(this);
    m_signalMessage = new SkpSignal(NULL, 0);
    m_myCode = SkpSocketManage::m_loginCode;
    connect(this, SIGNAL(sigMessage(QByteArray)), this, SLOT(onMessage(QByteArray)));
}

SkpTransferFileWidget::~SkpTransferFileWidget()
{
    delete ui;
}

void SkpTransferFileWidget::addItem(SkpTransferFileItemWidget *item)
{
    ui->verticalLayout->addWidget(item);
    m_itemList.append(item);
}

SkpTransferFileItemWidget *SkpTransferFileWidget::getItem(QString &md5)
{
    foreach (SkpTransferFileItemWidget *item, m_itemList) {
        if(md5 == item->md5())
            return item;
    }

    return NULL;
}

SkpTransferFileItemWidget *SkpTransferFileWidget::getPathItem(QString &path)
{
    foreach (SkpTransferFileItemWidget *item, m_itemList) {
        if(path == item->path())
            return item;
    }

    return NULL;
}

void SkpTransferFileWidget::deleteItem(QString &md5)
{
    SkpTransferFileItemWidget *item = getItem(md5);
    if(item)
        deleteItem(item);
}

void SkpTransferFileWidget::deleteItem(SkpTransferFileItemWidget *item)
{
    for(int i = 0; i < m_itemList.size(); i++) {
        SkpTransferFileItemWidget *itemFind = m_itemList.at(i);
        if(itemFind == item)
            m_itemList.removeAt(i);
    }
    ui->verticalLayout->removeWidget(item);
    if(ui->verticalLayout->isEmpty())
        setHidden(true);
    item->deleteLater();
}

void SkpTransferFileWidget::onDeleteItem(SkpTransferFileItemWidget *item)
{
    deleteItem(item);
}

void SkpTransferFileWidget::setPush(QString &path, quint64 friendCode)
{
    SkpTransferFileItemWidget *item = getPathItem(path);
    if(item)
        return;

    item = new SkpTransferFileItemWidget();
    connect(item, SIGNAL(sigDeleteItem(SkpTransferFileItemWidget*)), this, SLOT(onDeleteItem(SkpTransferFileItemWidget*)));
    item->setFixedHeight(50);
    item->setPush(path, friendCode);
    addItem(item);

    setHidden(false);
}

void SkpTransferFileWidget::setAck(Ack::MessageAck &ack)
{
    Message::MessageRequest request;
    request.set_class_(ack.class_());
    request.set_passage(ack.passage());
    request.set_friend_(m_myCode);
    Message::MessageRequestRange *range = request.add_index_range();
    range->set_start(0);
    range->set_end(0);

    m_signalMessage->skp_start();
    m_signalMessage->skp_set_ins(OrderLocalMessageGetMessage);
    m_signalMessage->skp_set_recv(ack.from());
    m_signalMessage->skp_append_messge(request.ByteSize(), &request);
    m_signalMessage->emit_sigKeep([this](const QByteArray &bytes)
    {
        emit sigMessage(bytes);
    });
}

void SkpTransferFileWidget::onMessage(const QByteArray &data)
{
    SkpLocalTLV tlvResponse;
    tlvResponse.skp_parse_buffer(data.data(), data.size());

    Message::MessageDataList messageDataList;
    tlvResponse.skp_Parse_message(&messageDataList);

    for(int i = 0; i < messageDataList.data_size(); i++) {
        Message::MessageData *messageData = messageDataList.mutable_data(i);
        int class_ = messageData->class_();
        int type = messageData->type();

        ///请求发送文件
        if(class_ == MessageClass_MemoryFile && type == MessageType_MemoryFileRequestToFriend) {
            MemoryFile::MemoryFileToFriend fileToFriend;
            fileToFriend.ParseFromArray(messageData->data_1().c_str(), messageData->data_1().size());

            SkpTransferFileItemWidget *item = new SkpTransferFileItemWidget();
            connect(item, SIGNAL(sigDeleteItem(SkpTransferFileItemWidget*)), this, SLOT(onDeleteItem(SkpTransferFileItemWidget*)));
            item->setFixedHeight(50);
            item->requestFileToFriend(fileToFriend);
            addItem(item);

            setHidden(false);
        }
        ///响应请求
        if(class_ == MessageClass_MemoryFile && type == MessageType_MemoryFileReplyToFriend) {

            MemoryFile::MemoryFileToFriend fileToFriend;
            fileToFriend.ParseFromArray(messageData->data_1().c_str(), messageData->data_1().size());

            QString md5 = QString::fromStdString(fileToFriend.md5());

            SkpTransferFileItemWidget *item = getItem(md5);
            if(!item) {
                return;
            }
            item->requestFileToFriendReply(fileToFriend);

            setHidden(false);
        }

        ///服务器申请
        if(class_ == MessageClass_MemoryFile && type == MessageType_MemoryFilePushRequest) {
            MemoryFile::MemoryFileData fileData;
            fileData.ParseFromArray(messageData->data_1().c_str(), messageData->data_1().size());

            QString md5 = QString::fromStdString(fileData.md5());

            SkpTransferFileItemWidget *item = getItem(md5);
            if(!item)
                return;

            item->requestPushFileToFriend(fileData);

            setHidden(false);
        }

        ///发送文件 通知到好友
        if(class_ == MessageClass_MemoryFile && type == MessageType_MemoryFilePush) {

            MemoryFile::MemoryFileData fileData;
            fileData.ParseFromArray(messageData->data_1().c_str(), messageData->data_1().size());

            QString md5 = QString::fromStdString(fileData.md5());

            SkpTransferFileItemWidget *item = getItem(md5);
            if(!item)
                return;
            item->pushFileToFriend(fileData);

            setHidden(false);
        }
    }
}
