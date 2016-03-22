#ifndef SKP_TRANSFER_FILE_H
#define SKP_TRANSFER_FILE_H

#include <QObject>
#include <QMutex>
#include <QThread>

typedef enum {
    FileDataStatusNull = 0,
    FileDataStatusPushing,
    FileDataStatusFinish
}FileServerStatus;

typedef enum {
    PullRstStatusSuccess = 0,//
    PullRstStatusError,//
    PullRstStatusPulling,//
    PullRstStatusStop,//
    PullRstStatusNULL,//
    PullRstStatusRate,

    PushRstStatusSuccess = 20,//
    PushRstStatusError,//
    PushRstStatusExist,//
    PushRstStatusPushing,//
    PushRstStatusStop,//
    PushRstStatusRequest,//
    PushRstStatusStartPush,//
    PushRstStatusRate,

    DeleteStatusSuccess = 50,
    DeleteStatusError,//

    FileToFriendStatusSuccess = 60,
    FileToFriendStatusError,

    FileStatusCheckMD5 = 70,//
    FileStatusCheckServer,//
    FileStatusNormal,//
    FileStatusDeleteStart,//
    FileStatusRequestFriend,//
}TransferRstStatus;

typedef enum {
    TransferTypeNULL = 0,
    TransferTypePush,
    TransferTypePull,
}TransferType;


QString clientStatus(int status);

class SkpSignal;
class SkpLocalTLV;

class SkpTransferFileThread : public QThread
{
public:
    SkpTransferFileThread();
    void add(quint64 size);
    void minus(quint64 size);
    quint64 value();
    quint64 m_size;
};

class SkpTransferFileItem : public QObject
{
    Q_OBJECT
public:
    SkpTransferFileItem();

public slots:
    void onGetFileMD5(const QString &path);
    void onReadFile(const QString &path, quint64 start, quint64 size);
    void onWriteFile(const QString &path, quint64 start, const QByteArray &byte);
signals:
    void sigFileMD5(const QString &MD5);
    void sigReadFile(quint64 start, const QByteArray &byte);
    void sigWriteFile(quint64 start, quint64 size);
};

class SkpTransferFile : public QObject
{
    Q_OBJECT

public:
    SkpTransferFile();
    ~SkpTransferFile();

    static void createThread(int number = 1);
    static QVector<SkpTransferFileThread *> m_threadVector;
    SkpTransferFileThread *getThread();

    SkpSignal *m_signal;
    SkpTransferFileItem *m_item;

    void addToFileThread();
    void getFileInfors(const QString &path);
    void setPath(const QString &path);
    void setName(const QString &name);
    void setMD5(const QString &md5);
    void setFileSize(quint64 fileSize);
    void setOffset(quint64 offset);
    QString &path() {return m_path;}

    void push();
    void pushRequest();
    void push(const QByteArray &byte, quint64 start, quint64 size);
    void pushFinish();
    void pull();
    void pull(quint64 start, quint64 size);
    void fileList();
    void start();
    void stop();
    void deleteFile();
    void fileToFriend();
    void fileToFriendReply();

    int m_type;

    QString m_path;
    QString m_name;
    QString m_md5;
    quint64 m_offset;
    quint64 m_fileSize;
    qint32 m_serverStatus;
    qint32 m_clientStatus;
    quint64 m_friendCode;

    quint64 m_onceSize;
    quint64 m_onceSendNumber;
    quint64 m_start;
    bool m_isError;
    bool m_isStop;

    quint64 m_allSendNumber;
    quint64 m_allrecvNumber;

    quint64 m_requestDataSize;

    quint32 m_lastTime;
    quint64 m_ratebytes;
    quint64 m_lastRatebytes;

public:
    void emit_GetFileMD5();

public slots:
    void onPull(const QByteArray &data);


signals:
    void sigReplyMD5(SkpTransferFile *transferFile);

    void sigPush(quint64 start, quint64 size);

    void sigPull(const QByteArray &data);

    void sigPush(SkpTransferFile *transferFile, qint32 Status);
    void sigFileList(SkpTransferFile *transferFile, const QByteArray &data);
    void sigPull(SkpTransferFile *transferFile, const QByteArray &data, qint32 status);
    void sigDelete(SkpTransferFile *transferFile, qint32 status);

    void sigFileToFriend(SkpTransferFile *transferFile, qint32 status);

signals:
    void sigGetFileMD5(const QString &path);
    void sigReadFile(const QString &path, quint64 start, quint64 size);
    void sigWriteFile(const QString &path, quint64 start, const QByteArray &byte);

public slots:
    void onFileMD5(const QString &md5);
    void onReadFile(quint64 start, const QByteArray &byte);
    void onWriteFile(quint64 start, quint64 size);
};


typedef struct SkpFileData_s SkpFileData_t;
struct SkpFileData_s
{
    quint32 type;
    QString path;
    QString name;
    QString md5;
    quint64 offset;
    quint64 length;
    quint32 serverStatus;
    quint32 clientStatus;
    quint32 proportion;
    quint64 rateByte;
    quint64 friendCode;
    SkpTransferFile *transferFile;
};

#endif // SKP_TRANSFER_FILE_H
