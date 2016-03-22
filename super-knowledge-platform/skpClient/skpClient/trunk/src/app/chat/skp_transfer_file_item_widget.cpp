#include "skp_transfer_file_item_widget.h"
#include "ui_skp_transfer_file_item_widget.h"
#include "skp_public_transfer_protocal.h"
#include "skp_transfer_file.h"
#include "skp_public_ins.h"
#include "skp_public_message_class.h"
#include "skp_app_utility.h"

#include "memory_file.pb.h"
#include "message.pb.h"

#include <QFileDialog>
#include <QDebug>

SkpTransferFileItemWidget::SkpTransferFileItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkpTransferFileItemWidget)
{
    ui->setupUi(this);
    m_transferFile = new SkpTransferFile();

    connect(m_transferFile, SIGNAL(sigReplyMD5(SkpTransferFile*)), this, SLOT(onReplyMD5(SkpTransferFile*)));
    connect(m_transferFile, SIGNAL(sigFileToFriend(SkpTransferFile*,qint32)), this, SLOT(onFileToFriend(SkpTransferFile*,qint32)));
    connect(m_transferFile, SIGNAL(sigPush(SkpTransferFile*,qint32)), this, SLOT(onPush(SkpTransferFile*,qint32)));
    connect(m_transferFile, SIGNAL(sigPull(SkpTransferFile*,QByteArray,qint32)), this, SLOT(onPull(SkpTransferFile*,QByteArray,qint32)));

}

SkpTransferFileItemWidget::~SkpTransferFileItemWidget()
{
    delete ui;
    delete m_transferFile;
}

QString &SkpTransferFileItemWidget::md5()
{
    return m_transferFile->m_md5;
}

QString &SkpTransferFileItemWidget::path()
{
    return m_transferFile->m_path;
}

void SkpTransferFileItemWidget::setPush(QString &path, quint64 friendCode)
{
    if(path.isEmpty())
        return;

    m_transferFile->getFileInfors(path);
    m_transferFile->m_friendCode = friendCode;
    m_transferFile->addToFileThread();


    ui->name_label->setText(m_transferFile->m_name);
    ui->progressBar_widget->setRange(0, m_transferFile->m_fileSize);

    m_transferFile->emit_GetFileMD5();
}

void SkpTransferFileItemWidget::onReplyMD5(SkpTransferFile *transferFile)
{
    m_transferFile->fileToFriend();
}

void SkpTransferFileItemWidget::onFileToFriend(SkpTransferFile *transferFile, qint32 status)
{
//    if(status == FileToFriendStatusSuccess) {
//        QjtMessageBox::information(this, tr("提示"), tr("请求成功"));
//    } else if(status == FileToFriendStatusError) {
//        QjtMessageBox::information(this, tr("提示"), tr("请求失败"));
//    }
}

void SkpTransferFileItemWidget::onPush(SkpTransferFile *transferFile, qint32 status)
{
    ui->progressBar_widget->setValue(m_transferFile->m_requestDataSize, m_transferFile->m_lastRatebytes);


    if(status == PushRstStatusRequest ||
            status == PushRstStatusPushing ||
            status == PushRstStatusRate ||
            status == PushRstStatusStop ||
            status == PushRstStatusError ||
            status == PushRstStatusExist ||
            status == PushRstStatusSuccess ||
            status == PushRstStatusStartPush) {

    }

    if(status == PushRstStatusSuccess)
        emit sigDeleteItem(this);
}

void SkpTransferFileItemWidget::requestFileToFriend(MemoryFile::MemoryFileToFriend &fileToFriend)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty())
        return;



    m_transferFile->setPath(dir);
    m_transferFile->setName(QString::fromStdString(fileToFriend.name()));
    m_transferFile->setMD5(QString::fromStdString(fileToFriend.md5()));
    m_transferFile->setOffset(0);
    m_transferFile->setFileSize(fileToFriend.lenth());
    m_transferFile->m_friendCode = fileToFriend.friend_();
    m_transferFile->addToFileThread();

    ui->name_label->setText(m_transferFile->m_name);
    ui->progressBar_widget->setRange(0, m_transferFile->m_fileSize);

    m_transferFile->fileToFriendReply();
}

void SkpTransferFileItemWidget::requestFileToFriendReply(MemoryFile::MemoryFileToFriend &fileToFriend)
{
    m_transferFile->pushRequest();
}

void SkpTransferFileItemWidget::requestPushFileToFriend(MemoryFile::MemoryFileData &fileDataIn)
{
    QString name = QString::fromStdString(fileDataIn.name());
    QString md5 = QString::fromStdString(fileDataIn.md5());
    quint64 offset = fileDataIn.offset();
    quint64 friendCode = fileDataIn.friend_();

    m_transferFile->setOffset(offset);
    m_transferFile->m_onceSize = 1024 * 128;
    m_transferFile->m_onceSendNumber = 100;
    m_transferFile->m_start = 0;
    m_transferFile->m_isError = false;

    m_transferFile->m_isStop = false;
    m_transferFile->m_allSendNumber = 0;
    m_transferFile->m_allrecvNumber = 0;

    m_transferFile->m_requestDataSize = 0;
    m_transferFile->m_lastTime = 0;
    m_transferFile->m_ratebytes = 0;
    m_transferFile->m_lastRatebytes = 0;
}

void SkpTransferFileItemWidget::pushFileToFriend(MemoryFile::MemoryFileData &fileDataIn)
{
    QString name = QString::fromStdString(fileDataIn.name());
    QString md5 = QString::fromStdString(fileDataIn.md5());
    quint64 offset = fileDataIn.offset();
    quint64 length = fileDataIn.lenth();
    quint64 friendCode = fileDataIn.friend_();

    quint64 start = offset - m_transferFile->m_offset;
    m_transferFile->pull(start, length);
}

void SkpTransferFileItemWidget::onPull(SkpTransferFile *transferFile, const QByteArray &data, qint32 status)
{
    ui->progressBar_widget->setValue(m_transferFile->m_requestDataSize, m_transferFile->m_lastRatebytes);


    if(status == PullRstStatusStop ||
            status == PullRstStatusPulling ||
            status == PullRstStatusRate ||
            status == PullRstStatusError ||
            status == PullRstStatusNULL ||
            status == PullRstStatusSuccess) {

    }

    if(status == PullRstStatusSuccess)
        emit sigDeleteItem(this);
}
