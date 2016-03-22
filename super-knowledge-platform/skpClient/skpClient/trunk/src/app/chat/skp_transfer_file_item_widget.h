#ifndef SKP_TRANSFER_FILE_ITEM_WIDGET_H
#define SKP_TRANSFER_FILE_ITEM_WIDGET_H

#include <QWidget>

namespace Ui {
class SkpTransferFileItemWidget;
}

namespace MemoryFile {
class MemoryFileToFriend;
class MemoryFileData;
}

namespace Message {
class MessageData;
}

typedef struct SkpFileData_s SkpFileData_t;
class SkpTransferFile;

class SkpTransferFileItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SkpTransferFileItemWidget(QWidget *parent = 0);
    ~SkpTransferFileItemWidget();
    void setPush(QString &path, quint64 friendCode);
    void requestFileToFriend(MemoryFile::MemoryFileToFriend &fileToFriend);
    void requestFileToFriendReply(MemoryFile::MemoryFileToFriend &fileToFriend);
    void requestPushFileToFriend(MemoryFile::MemoryFileData &fileData);
    void pushFileToFriend(MemoryFile::MemoryFileData &fileData);
    QString &md5();
    QString &path();

private:
    Ui::SkpTransferFileItemWidget *ui;
    SkpTransferFile *m_transferFile;

public slots:
    void onFileToFriend(SkpTransferFile *transferFile, qint32 status);
    void onPush(SkpTransferFile *transferFile, qint32 status);
    void onPull(SkpTransferFile *transferFile, const QByteArray &data, qint32 status);

signals:
    void sigDeleteItem(SkpTransferFileItemWidget *item);

public slots:
    void onReplyMD5(SkpTransferFile *transferFile);
};

#endif // SKP_TRANSFER_FILE_ITEM_WIDGET_H
