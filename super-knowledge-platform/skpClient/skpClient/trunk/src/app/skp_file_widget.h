#ifndef SKP_FILE_WIDGET_H
#define SKP_FILE_WIDGET_H

#include <QWidget>

class QTableWidgetItem;
class QMenu;
class QAction;
class SkpSignal;
class SkpChatWidget;

typedef struct SkpDataCallback_s SkpDataCallback_t;

namespace MemoryFile {
    class MemoryFileToFriend;
    class MemoryFileData;
}

namespace Ui {
class SkpFileWidget;
}

class SkpTransferFile;

class SkpFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SkpFileWidget(QWidget *parent = 0);
    ~SkpFileWidget();

private:
    Ui::SkpFileWidget *ui;
    QMenu *m_menu;
    QAction *m_pullAction;
    QAction *m_startAction;
    QAction *m_stopAction;
    QAction *m_deleteAction;

    QColor m_mouseMoveColor;
    QColor m_defaultBackgroundColor;
    int m_currMouseRow;

    SkpSignal *m_signal;
    SkpSignal *m_signalCallback;

    QHash<quint64, SkpChatWidget *> m_friendChatHash;
public:
    QString serverStatus(int status);
    void pull(int row, QString &dir);
    void start(int row);
    void stop(int row);
    void deleteFile(int row);
    void setRowColor(int row, QColor &color);
    int isExist(QString &path, QString &md5, qint32 clientStatus);
    int isExistFileToFriend(QString &name, QString &md5, quint64 friendCode);
    int isExistMD5(QString &md5);
    int isExistPath(QString &path);
    void selectAndShowRow(int row);
    void appendRowData(void *data);
    void insertRowData(int row, void *data);
    void *getRowData(int row);
    void updateRowData(int row);
    void appendPushFile(QStringList &files);
    void registerCallbackChat(QByteArray &bytes);
    SkpChatWidget *setFriendChat(QString &friendcode, QString &friendName);
    SkpChatWidget *friendChat(QString &friendcode, QString &friendName);
    SkpChatWidget *friendChat(QString &friendcode);
signals:
    void sigRefreshFriend(const QByteArray &data);
    void sigRegister(quint64 classType, std::function<void(QByteArray &)> func);
    void sigFriendChat(const QByteArray &data);
public slots:
    void onPushFile();
    void onReplyMD5(SkpTransferFile *transferFile);
    void onPush(SkpTransferFile *transferFile, qint32 Status);
    void onRefresh();
    void onFileList(SkpTransferFile *transferFile, const QByteArray &data);
    void onTtemDoubleClicked(QTableWidgetItem *item);
    void onPull(SkpTransferFile *transferFile, const QByteArray &data, qint32 status);
    void onCustomContextMenuRequested(const QPoint &pos);
    void onCellEntered(int row, int column);
    void onCellClicked(int row, int column);
    void onPullAction();
    void onStartAction();
    void onStopAction();
    void onDeleteAction();
    void onDelete(SkpTransferFile *transferFile, qint32 status);

    void onRefreshFriend();
    void onRefreshFriend(const QByteArray &data);

    void onFriendChat(const QByteArray &data);
};

#endif // SKP_FILE_WIDGET_H
