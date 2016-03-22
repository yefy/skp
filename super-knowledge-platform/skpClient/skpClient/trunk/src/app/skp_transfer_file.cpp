#include "skp_transfer_file.h"
#include "skp_core_signal.h"
#include "skp_core_tlv.h"
#include "skp_app_utility.h"
#include "memory_file.pb.h"
#include "skp_public_ins.h"

#include <QFile>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QDebug>
#include <time.h>

#define IS_KEEP 0

#define PULL_TO_FILE 0


QString clientStatus(int status)
{
    if(status == FileStatusNormal)//
        return QObject::tr("正常");
    if(status == PushRstStatusPushing )//FileStatusPushing
        return QObject::tr("正在上传");
    if(status == PushRstStatusError )//FileStatusPushError
        return QObject::tr("上传失败");
    if(status == PushRstStatusSuccess )//FileStatusPushSuccess
        return QObject::tr("上传成功");
    if(status == PushRstStatusStop )//FileStatusPushStop
        return QObject::tr("上传暂停");
    if(status == PullRstStatusPulling )//FileStatusPulling
        return QObject::tr("正在下载");
    if(status == PullRstStatusError )//FileStatusPullError
        return QObject::tr("下载失败");
    if(status == PullRstStatusSuccess )//FileStatusPullSuccess
        return QObject::tr("下载成功");
    if(status == PullRstStatusStop )//FileStatusPullStop
        return QObject::tr("下载暂停");
    if(status == DeleteStatusError )//FileStatusDeleteError
        return QObject::tr("删除失败");
    if(status == PushRstStatusExist) //FileStatusExist
        return QObject::tr("已经存在");
    if(status == FileStatusCheckMD5)//
        return QObject::tr("正在获取MD5");
    if(status == FileStatusCheckServer)//
        return QObject::tr("正在检查后台文件");
    if(status == PushRstStatusStartPush )//FileStatusStartPush
        return QObject::tr("开始上传");
    if(status == PushRstStatusRequest )//FileStatusPushRequest
        return QObject::tr("请求上传");
    if(status == FileStatusDeleteStart)//
        return QObject::tr("正在删除");
    if(status == PullRstStatusNULL )//FileStatusPullNULL
        return QObject::tr("服务器文件不存在");
    if(status == FileStatusRequestFriend)
        return QObject::tr("正在请求");
    if(status == PullRstStatusRate)
        return QObject::tr("正在下载");
    if(status == PushRstStatusRate)
        return QObject::tr("正在上传");

    return QObject::tr("NULL");
}


//================================ SkpTransferFileThread
SkpTransferFileThread::SkpTransferFileThread() :
    QThread(0),
    m_size(0)
{

}

void SkpTransferFileThread::add(quint64 size)
{
    m_size += size;
}

void SkpTransferFileThread::minus(quint64 size)
{
    m_size -= size;
}

quint64 SkpTransferFileThread::value()
{
    return m_size;
}


QVector<SkpTransferFileThread *> SkpTransferFile::m_threadVector;


//================================ SkpTransferFileItem
SkpTransferFileItem::SkpTransferFileItem()
{

}

void SkpTransferFileItem::onGetFileMD5(const QString &path)
{
    QString MD5;
    SkpUtility::skp_get_file_md5(path, MD5);

    emit sigFileMD5(MD5);
}

void SkpTransferFileItem::onReadFile(const QString &path, quint64 start, quint64 size)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    file.seek(start);
    QByteArray byte = file.read(size);
    file.close();

    emit sigReadFile(start, byte);
}

void SkpTransferFileItem::onWriteFile(const QString &path, quint64 start, const QByteArray &byte)
{
#if PULL_TO_FILE
        QFile file(path);
        file.open(QIODevice::WriteOnly);
        file.seek(start);
        file.write(byte);
        file.close();
#endif

        emit sigWriteFile(start, byte.size());
}

//================================ SkpTransferFile
SkpTransferFile::SkpTransferFile() :
    QObject(0),
    m_type(TransferTypeNULL),
    m_path(""),
    m_name(""),
    m_md5(""),
    m_offset(0),
    m_fileSize(0),
    m_serverStatus(0),
    m_clientStatus(0),
    m_friendCode(0),
    m_onceSize(0),
    m_onceSendNumber(0),
    m_start(0),
    m_isError(false),
    m_isStop(false),
    m_allSendNumber(0),
    m_allrecvNumber(0),
    m_requestDataSize(0),
    m_lastTime(0),
    m_ratebytes(0),
    m_lastRatebytes(0)
{
    m_signal = new SkpSignal(NULL, 0);
    m_signal->skp_set_order_type(OrderTypeRequest);

    connect(this, SIGNAL(sigPull(QByteArray)), this, SLOT(onPull(QByteArray)));


    m_item = new SkpTransferFileItem();
    connect(this, SIGNAL(sigGetFileMD5(QString)), m_item, SLOT(onGetFileMD5(QString)));
    connect(this, SIGNAL(sigReadFile(QString,quint64,quint64)), m_item, SLOT(onReadFile(QString,quint64,quint64)));
    connect(this, SIGNAL(sigWriteFile(QString,quint64,QByteArray)), m_item, SLOT(onWriteFile(QString,quint64,QByteArray)));

    connect(m_item, SIGNAL(sigFileMD5(QString)), this, SLOT(onFileMD5(QString)));
    connect(m_item, SIGNAL(sigReadFile(quint64,QByteArray)), this, SLOT(onReadFile(quint64,QByteArray)));
    //connect(m_item, SIGNAL(sigWriteFile(quint64,quint64)), this, SLOT(onWriteFile(quint64,quint64)));

}

SkpTransferFile::~SkpTransferFile()
{
    m_item->deleteLater();
}

void SkpTransferFile::createThread(int number)
{
    for(int i = 0; i < number; i ++) {
        SkpTransferFileThread *thread = new SkpTransferFileThread();
        thread->start();
        m_threadVector.append(thread);
    }
}

SkpTransferFileThread *SkpTransferFile::getThread()
{
    if(m_threadVector.isEmpty())
        return NULL;

    quint64 min = 0x0FFFFFFFFFFFFFFF;
    SkpTransferFileThread *tempThread = NULL;
    int number = -1;
    for(int i = 0; i < m_threadVector.size(); i++) {
        SkpTransferFileThread *thread = m_threadVector.at(i);

        if(thread->value() < min) {
            min = thread->value();
            tempThread = thread;
            number = i;
        }
    }

    if(!tempThread)
        tempThread = m_threadVector.at(0);

    return tempThread;
}

void SkpTransferFile::addToFileThread()
{
    SkpTransferFileThread *thread = getThread();
    assert(thread);
    if(thread) {
        thread->add(m_fileSize);
        m_item->moveToThread(thread);
    }
}

void SkpTransferFile::getFileInfors(const QString &path)
{
    m_path = path;
    QFileInfo fileInfo(path);
    m_name = fileInfo.fileName();
    m_fileSize = fileInfo.size();
}

void SkpTransferFile::setPath(const QString &path)
{
    m_path = path;
}

void SkpTransferFile::setName(const QString &name)
{
    m_name = name;
}

void SkpTransferFile::setMD5(const QString &md5)
{
    m_md5 = md5;
}

void SkpTransferFile::setFileSize(quint64 fileSize)
{
    m_fileSize = fileSize;
}

void SkpTransferFile::setOffset(quint64 offset)
{
    m_offset = offset;
}



void SkpTransferFile::push()
{
    qDebug() << "push";
    m_type = TransferTypePush;

    MemoryFile::MemoryFileMD5 fileMD5;
    fileMD5.set_md5(m_md5.toStdString());

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalMemoryFileIsExist);
    m_signal->skp_append_messge(fileMD5.ByteSize(), &fileMD5);

#if IS_KEEP
    m_signal->emit_sigKeep([this](const QByteArray &bytes)
                       #else
    m_signal->emit_sigMoment([this](const QByteArray &bytes)
                         #endif
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());


        if(tlvResponse.skp_status()) {
            MemoryFile::MemoryFileMD5 fileMD5;
            tlvResponse.skp_Parse_message(&fileMD5);

            QString md5 = QString::fromStdString(fileMD5.md5());

            if(md5 != m_md5) {
                qDebug() << "push error";
                emit sigPush(this, PushRstStatusError);
                return;
            }

            emit sigPush(this, PushRstStatusRequest);
            pushRequest();
        } else {
            MemoryFile::MemoryFileData fileData;
            tlvResponse.skp_Parse_message(&fileData);

            //m_md5 = QString::fromStdString(fileData.md5());
            //m_path = QString::fromStdString(fileData.name());
            //m_fileSize = fileData.lenth();
            //m_offset = fileData.offset();

            qDebug() << "push exist";
            emit sigPush(this, PushRstStatusExist);
        }
    });
}

void SkpTransferFile::pushRequest()
{
    qDebug() << "pushRequest";
    MemoryFile::MemoryFileData fileData;
    fileData.set_md5(m_md5.toStdString());
//    fileData.set_name(m_path.toStdString());
    fileData.set_name(m_name.toStdString());
//    fileData.set_name(m_name.toUtf8());
//    fileData.set_name(m_name.toLocal8Bit());
    fileData.set_lenth(m_fileSize);
    if(m_friendCode)
        fileData.set_friend_(m_friendCode);

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalMemoryFilePushRuest);
    m_signal->skp_append_messge(fileData.ByteSize(), &fileData);

#if IS_KEEP
    m_signal->emit_sigKeep([this](const QByteArray &bytes)
                       #else
    m_signal->emit_sigMoment([this](const QByteArray &bytes)
                         #endif
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes);

        if(!tlvResponse.skp_status()) {

            MemoryFile::MemoryFileData fileData;
            tlvResponse.skp_Parse_message(&fileData);

            m_offset = fileData.offset();

            m_onceSize = 1024 * 128;
            m_onceSendNumber = 1;
            m_start = 0;
            m_isError = false;

            m_isStop = false;
            m_allSendNumber = 0;
            m_allrecvNumber = 0;

            m_requestDataSize = 0;
            m_lastTime = 0;
            m_lastRatebytes = 0;
            m_ratebytes= 0;

            qDebug() << "start  push";

            emit sigPush(this, PushRstStatusStartPush);

            for(int i = 0; i < m_onceSendNumber; i++) {
                quint64 size = m_onceSize;

                if(m_start + size > m_fileSize) {
                    size -= ((m_start + size) - m_fileSize);
                }

                quint64 diff = (m_fileSize - (m_start + size));

                if(diff > 0 && diff < m_onceSize) {
                    size += diff;
                }

                emit sigReadFile(m_path, m_start, size);
                m_allSendNumber++;
                m_start += size;
                if(m_start == m_fileSize)
                    break;
            }

        } else {
            qDebug() << "push  error";
            emit sigPush(this, PushRstStatusError);
        }
    });
}

void SkpTransferFile::push(const QByteArray &byte, quint64 start, quint64 size)
{
    MemoryFile::MemoryFileData fileData;
    fileData.set_md5(m_md5.toStdString());
//    fileData.set_name(m_path.toStdString());
    fileData.set_name(m_name.toStdString());
    fileData.set_lenth(size);
    if(m_friendCode)
        fileData.set_friend_(m_friendCode);

    quint64 offset = m_offset + start;

    if(offset < m_offset ||
            offset > m_offset + m_fileSize ||
            offset + size > m_offset + m_fileSize||
            size == 0) {
        qDebug() << "offset error";
        return;
    }

    fileData.set_offset(offset);
    fileData.set_buffer(byte.data(), byte.size());

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalMemoryFilePush);
    m_signal->skp_append_messge(fileData.ByteSize(), &fileData);

#if IS_KEEP
    m_signal->emit_sigKeep([this](const QByteArray &bytes)
                       #else
    m_signal->emit_sigMoment([this](const QByteArray &bytes)
                         #endif
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());

        if(!tlvResponse.skp_status()) {

            MemoryFile::MemoryFileData fileData;
            tlvResponse.skp_Parse_message(&fileData);

            m_requestDataSize += fileData.lenth();

            if(!m_lastTime) {
                m_lastTime = time(NULL);
            }
            if(time(NULL) - m_lastTime >= 1) {
                m_lastTime = time(NULL);
                m_lastRatebytes = m_ratebytes;
                m_ratebytes = 0;
                emit sigPush(this, PushRstStatusRate);
            } else {
                m_ratebytes += fileData.lenth();
            }


            QString md5 = QString::fromStdString(fileData.md5());
            if(md5 != m_md5) {
                qDebug() << "push error";
                m_isError = true;
                emit sigPush(this, PushRstStatusError);
                return;
            }

            if(m_requestDataSize < m_fileSize)
                emit sigPush(this, PushRstStatusPushing);

            if(m_requestDataSize == m_fileSize) {

                if(!m_lastRatebytes)
                    m_lastRatebytes = m_ratebytes;
                SkpTransferFileThread *transferFileThread = (SkpTransferFileThread *)m_item->thread();
                transferFileThread->minus(m_fileSize);
                pushFinish();
                return;
            } else {
                if(m_start == m_fileSize)
                    return;
                m_allrecvNumber++;

                if(m_isStop) {
                    if(m_allrecvNumber == m_allSendNumber) {\
                        emit sigPush(this, PushRstStatusStop);
                    }
                    return;
                }

                quint64 size = m_onceSize;

                if(m_start + size > m_fileSize) {
                    size -= ((m_start + size) - m_fileSize);
                }

                quint64 diff = (m_fileSize - (m_start + size));

                if(diff > 0 && diff < m_onceSize) {
                    size += diff;
                }

                emit sigReadFile(m_path, m_start, size);
                m_allSendNumber++;
                m_start += size;
            }
        } else {
            if(!m_isError) {
                m_isError = true;
                SkpTransferFileThread *transferFileThread = (SkpTransferFileThread *)m_item->thread();
                transferFileThread->minus(m_fileSize);
                qDebug() << "push error";
                emit sigPush(this, PushRstStatusError);
            }
        }
    });


}

void SkpTransferFile::pushFinish()
{
    qDebug() << "pushFinish";
    MemoryFile::MemoryFileMD5 fileMD5;
    fileMD5.set_md5(m_md5.toStdString());

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalMemoryFilePushOK);
    m_signal->skp_append_messge(fileMD5.ByteSize(), &fileMD5);

#if IS_KEEP
    m_signal->emit_sigKeep([this](const QByteArray &bytes)
                       #else
    m_signal->emit_sigMoment([this](const QByteArray &bytes)
                         #endif
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());

        if(tlvResponse.skp_status()) {
            emit sigPush(this, PushRstStatusError);
            return;
        }

        MemoryFile::MemoryFileMD5 fileMD5;
        tlvResponse.skp_Parse_message(&fileMD5);

        QString md5 = QString::fromStdString(fileMD5.md5());
        if(md5 != m_md5) {
            emit sigPush(this, PushRstStatusError);
            return;
        }

        emit sigPush(this, PushRstStatusPushing);

        emit sigPush(this, PushRstStatusSuccess);

    });
}

void SkpTransferFile::pull()
{
    m_type = TransferTypePull;

    MemoryFile::MemoryFileMD5 fileMD5;
    fileMD5.set_md5(m_md5.toStdString());

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalMemoryFileIsExist);
    m_signal->skp_append_messge(fileMD5.ByteSize(), &fileMD5);


#if IS_KEEP
    m_signal->emit_sigKeep([this](const QByteArray &bytes)
                       #else
    m_signal->emit_sigMoment([this](const QByteArray &bytes)
                         #endif
    {

        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());

        if(tlvResponse.skp_status()) {
            emit sigPull(this, NULL, PullRstStatusNULL);
        } else {
            MemoryFile::MemoryFileData fileData;
            tlvResponse.skp_Parse_message(&fileData);

            quint64 fileSize = fileData.lenth();
            quint64 offset = fileData.offset();

            if(m_fileSize != fileSize || m_offset != offset) {
                emit sigPull(this, NULL, PullRstStatusError);
                return;
            }

            m_onceSize = 1024 * 128;
            m_onceSendNumber = 1;
            m_start = 0;
            m_isError = false;

            m_isStop = false;
            m_allSendNumber = 0;
            m_allrecvNumber = 0;

            m_requestDataSize = 0;
            m_lastTime = 0;
            m_ratebytes = 0;
            m_lastRatebytes = 0;


            for(int i = 0; i < m_onceSendNumber; i++) {
                quint64 size = m_onceSize;

                if(m_start + size > m_fileSize) {
                    size -= ((m_start + size) - m_fileSize);
                }

                quint64 diff = (m_fileSize - (m_start + size));

                if(diff > 0 && diff < m_onceSize) {
                    size += diff;
                }

                pull(m_start, size);
                m_allSendNumber++;
                m_start += size;
                if(m_start == m_fileSize)
                    break;
            }
        }
    });
}

void SkpTransferFile::pull(quint64 start, quint64 size)
{
    MemoryFile::MemoryFileData fileData;
    fileData.set_md5(m_md5.toStdString());
//    fileData.set_name(m_path.toStdString());
    fileData.set_name(m_name.toStdString());
    fileData.set_lenth(size);

    quint64 offset = m_offset + start;
    if(offset < m_offset ||
            offset > m_offset + m_fileSize ||
            offset + size > m_offset + m_fileSize||
            size == 0) {
        qDebug() << "offset error";
        return;
    }

    fileData.set_offset(offset);

    m_signal->skp_start();
    if(m_friendCode)
        m_signal->skp_set_recv(m_friendCode);
    m_signal->skp_set_ins(OrderLocalMemoryFilePull);
    m_signal->skp_append_messge(fileData.ByteSize(), &fileData);
#if IS_KEEP
    m_signal->emit_sigKeep([this](const QByteArray &bytes)
                       #else
    m_signal->emit_sigMoment([this](const QByteArray &bytes)
                         #endif
    {
        emit sigPull(bytes);
    });
}

void SkpTransferFile::fileList()
{
    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalMemoryFileFileList);

#if IS_KEEP
    m_signal->emit_sigKeep([this](const QByteArray &bytes)
                       #else
    m_signal->emit_sigMoment([this](const QByteArray &bytes)
                         #endif
    {
        emit sigFileList(this, bytes);
    });
}

void SkpTransferFile::start()
{
    m_isStop = false;

    for(int i = 0; i < m_onceSendNumber; i++) {
        quint64 size = m_onceSize;

        if(m_start + size > m_fileSize) {
            size -= ((m_start + size) - m_fileSize);
        }

        quint64 diff = (m_fileSize - (m_start + size));

        if(diff > 0 && diff < m_onceSize) {
            size += diff;
        }

        if(m_type == TransferTypePush) {
            emit sigPush(m_start, size);
        } else if(m_type == TransferTypePull) {
            pull(m_start, size);
        }
        m_allSendNumber++;
        m_start += size;
        if(m_start == m_fileSize)
            break;
    }
}

void SkpTransferFile::stop()
{
    m_isStop = true;
}
void SkpTransferFile::deleteFile()
{
    MemoryFile::MemoryFileMD5 fileMD5;
    fileMD5.set_md5(m_md5.toStdString());

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalMemoryFileDelete);
    m_signal->skp_append_messge(fileMD5.ByteSize(), &fileMD5);

#if IS_KEEP
    m_signal->emit_sigKeep([this](const QByteArray &bytes)
#else
    m_signal->emit_sigMoment([this](const QByteArray &bytes)
#endif
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());


        if(tlvResponse.skp_status()) {
            emit sigDelete(this, DeleteStatusError);
        } else {
            emit sigDelete(this, DeleteStatusSuccess);
        }
    });
}

void SkpTransferFile::emit_GetFileMD5()
{
    emit sigGetFileMD5(m_path);
}

void SkpTransferFile::onPull(const QByteArray &data)
{
    SkpLocalTLV tlvResponse;
    tlvResponse.skp_parse_buffer(data.data(), data.size());

    MemoryFile::MemoryFileData fileData;
    tlvResponse.skp_Parse_message(&fileData);
    if(!tlvResponse.skp_status()) {
        m_requestDataSize += fileData.lenth();

        if(!m_lastTime) {
            m_lastTime = time(NULL);
        }
        if(time(NULL) - m_lastTime >= 1) {
            m_lastTime = time(NULL);
            m_lastRatebytes = m_ratebytes;
            m_ratebytes = 0;
            emit sigPull(this, NULL, PullRstStatusRate);
        } else {
            m_ratebytes += fileData.lenth();
        }

        quint64 start = fileData.offset() - m_offset;
        emit sigWriteFile(m_path, start, QByteArray(fileData.buffer().c_str(), fileData.buffer().size()));


        if(m_requestDataSize < m_fileSize)
            emit sigPull(this, NULL, PullRstStatusPulling);

        if(m_requestDataSize == m_fileSize) {
            if(!m_lastRatebytes)
                m_lastRatebytes = m_ratebytes;
            QString md5 = QString::fromStdString(fileData.md5());

            if(md5 != m_md5) {
                emit sigPull(this, NULL, PullRstStatusError);
                return;
            }

#if PULL_TO_FILE
            QString pullMD5;
            SkpUtility::skp_get_file_md5(m_path, pullMD5);

            if(md5 != pullMD5) {
                emit sigPull(this, NULL, PullRstStatusError);
                return;
            }
#endif

            SkpTransferFileThread *transferFileThread = (SkpTransferFileThread *)m_item->thread();
            transferFileThread->minus(m_fileSize);
            emit sigPull(this, NULL, PullRstStatusPulling);

            emit sigPull(this, NULL, PullRstStatusSuccess);
            return;
        } else {
            if(m_friendCode)
                return;

            if(m_start == m_fileSize)
                return;

            m_allrecvNumber++;

            if(m_isStop) {
                if(m_allrecvNumber == m_allSendNumber)
                    emit sigPull(this, NULL, PullRstStatusStop);
                return;
            }

            quint64 size = m_onceSize;

            if(m_start + size > m_fileSize) {
                size -= ((m_start + size) - m_fileSize);
            }

            quint64 diff = (m_fileSize - (m_start + size));

            if(diff > 0 && diff < m_onceSize) {
                size += diff;
            }

            pull(m_start, size);
            m_allSendNumber++;
            m_start += size;
        }
    }else {
        if(!m_isError) {
            m_isError = true;
            SkpTransferFileThread *transferFileThread = (SkpTransferFileThread *)m_item->thread();
            transferFileThread->minus(m_fileSize);
            emit sigPull(this, NULL, PullRstStatusError);
        }
    }
}

void SkpTransferFile::fileToFriend()
{
    MemoryFile::MemoryFileToFriend fileData;
    fileData.set_name(m_name.toStdString());
    fileData.set_md5(m_md5.toStdString());
    fileData.set_lenth(m_fileSize);
    fileData.set_friend_(m_friendCode);

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalMemoryFileToFriend);
    m_signal->skp_append_messge(fileData.ByteSize(), &fileData);

    m_signal->emit_sigKeep([this](const QByteArray &bytes)
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());

        if(tlvResponse.skp_status()) {
            emit sigFileToFriend(this, FileToFriendStatusError);
        } else {
            emit sigFileToFriend(this, FileToFriendStatusSuccess);
        }
    });
}

void SkpTransferFile::fileToFriendReply()
{
    MemoryFile::MemoryFileToFriend fileData;
    fileData.set_name(m_name.toStdString());
    fileData.set_md5(m_md5.toStdString());
    fileData.set_lenth(m_fileSize);
    fileData.set_friend_(m_friendCode);

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalMemoryFileToFriendReply);
    m_signal->skp_append_messge(fileData.ByteSize(), &fileData);

    m_signal->emit_sigKeep([this](const QByteArray &bytes)
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());

//        if(tlvResponse.skp_status()) {
//            emit sigFileToFriend(this, FileToFriendStatusError);
//        } else {
//            emit sigFileToFriend(this, FileToFriendStatusSuccess);
//        }
    });
}


void SkpTransferFile::onFileMD5(const QString &md5)
{
    m_md5 = md5;
    emit sigReplyMD5(this);
}

void SkpTransferFile::onReadFile(quint64 start, const QByteArray &byte)
{
    push(byte, start, byte.size());
}

void SkpTransferFile::onWriteFile(quint64 start, quint64 size)
{

}
