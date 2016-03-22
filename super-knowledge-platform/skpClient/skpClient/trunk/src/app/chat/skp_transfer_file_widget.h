#ifndef SKP_TRANSFER_FILE_WIDGET_H
#define SKP_TRANSFER_FILE_WIDGET_H

#include <QWidget>

namespace Ui {
class SkpTransferFileWidget;
}

namespace Message {
class MessageData;
}

namespace Ack {
class MessageAck;
}

namespace MemoryFile {
class MemoryFileToFriend;
}

class SkpTransferFileItemWidget;
class SkpSignal;

class SkpTransferFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SkpTransferFileWidget(QWidget *parent = 0);
    ~SkpTransferFileWidget();
    void setPush(QString &path, quint64 friendCode);
    void setAck(Ack::MessageAck &ack);

private:
    Ui::SkpTransferFileWidget *ui;
    void addItem(SkpTransferFileItemWidget *item);
    SkpTransferFileItemWidget *getItem(QString &md5);
    SkpTransferFileItemWidget *getPathItem(QString &path);
    void deleteItem(QString &md5);
    void deleteItem(SkpTransferFileItemWidget *item);

    QList<SkpTransferFileItemWidget *> m_itemList;
    SkpSignal *m_signalMessage;
    quint64 m_myCode;

signals:
    void sigMessage(const QByteArray &data);

public slots:
    void onMessage(const QByteArray &data);
    void onDeleteItem(SkpTransferFileItemWidget *item);
};

#endif // SKP_TRANSFER_FILE_WIDGET_H
