#include "skp_file_widget.h"
#include "ui_skp_file_widget.h"
#include "skp_transfer_file.h"
#include "skp_local_tlv.h"
#include "skp_core_signal.h"
#include "skp_core_manage.h"
#include "skp_socket_manage.h"
#include "skp_chat_widget.h"
#include "friend.pb.h"
#include "memory_file.pb.h"
#include "ack.pb.h"
#include "message.pb.h"
#include "skp_public_ins.h"
#include "skp_public_message_class.h"
#include "skp_app_utility.h"


#include <QFileDialog>
#include <QDebug>
#include <QTableWidgetItem>
#include <QMenu>
#include <QAction>
#include <QStyledItemDelegate>
#include <QScrollBar>
#include "QjtMessageBox.h"

typedef enum {
    TableHeaderTypeName = 0,
    TableHeaderTypeMD5,
    TableHeaderTypeOffset,
    TableHeaderTypeLength,
    TableHeaderTypeServerStatus,
    TableHeaderTypeClientStatus,
    TableHeaderTypeProportion,
}TableHeaderType;


class NoFocusDelegate : public QStyledItemDelegate
{
public:
    NoFocusDelegate();
    void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const;
};

NoFocusDelegate::NoFocusDelegate() :
    QStyledItemDelegate(0)
{

}

void NoFocusDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus)
    {
        itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, itemOption, index);
}


SkpFileWidget::SkpFileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkpFileWidget),
    m_mouseMoveColor(QColor(193, 210, 240, 50)),
    m_defaultBackgroundColor(QColor(255, 255, 255)),
    m_currMouseRow(-1)
{
    ui->setupUi(this);

    SkpTransferFile::createThread(5);

    m_signal = new SkpSignal(NULL, 0);
    m_signalCallback = new SkpSignal(NULL, 0);

    auto func = std::bind(&SkpFileWidget::registerCallbackChat, this, std::placeholders::_1);
    m_signal->emit_register_ack(MessageClass_MemoryFile, func);
    m_signal->emit_register_ack(MessageClass_Chat, func);

    connect(this, SIGNAL(sigFriendChat(QByteArray)), this, SLOT(onFriendChat(QByteArray)));

    connect(ui->pushFile_pushButton,SIGNAL(clicked()), this, SLOT(onPushFile()));
    connect(ui->refresh_pushButton, SIGNAL(clicked()), this, SLOT(onRefresh()));

    QStringList header;
    header << tr("name") << tr("md5") << tr("offset") << tr("lenth") << tr("服务器文件状态") << tr("请求状态") << tr("%");

    ui->tableWidget->setColumnCount(header.size());
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget->setShowGrid(false); //设置不显示格子线
    ui->tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget->setItemDelegate(new NoFocusDelegate());
    ui->tableWidget->setMouseTracking(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenuRequested(QPoint)));
    connect(ui->tableWidget, SIGNAL(cellEntered(int,int)), this, SLOT(onCellEntered(int,int)));
    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(onCellClicked(int,int)));



    header.clear();
    header << tr("code") << tr("name") << tr("password");

    ui->friend_tableWidget->setColumnCount(header.size());
    ui->friend_tableWidget->setHorizontalHeaderLabels(header);
    ui->friend_tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->friend_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->friend_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->friend_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->friend_tableWidget->setShowGrid(false); //设置不显示格子线
    ui->friend_tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->friend_tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->friend_tableWidget->horizontalHeader()->setHighlightSections(false);
    ui->friend_tableWidget->setItemDelegate(new NoFocusDelegate());
    ui->friend_tableWidget->setMouseTracking(true);
    ui->friend_tableWidget->horizontalHeader()->setHighlightSections(false);
    connect(ui->refreshFriend_pushButton, SIGNAL(clicked()), this, SLOT(onRefreshFriend()));
    connect(this, SIGNAL(sigRefreshFriend(QByteArray)), this, SLOT(onRefreshFriend(QByteArray)));

    connect(ui->friend_tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onTtemDoubleClicked(QTableWidgetItem*)));


    m_menu = new QMenu(ui->tableWidget);
    m_pullAction = new QAction("down", this);
    connect(m_pullAction, SIGNAL(triggered()), this, SLOT(onPullAction()));
    m_menu->addAction(m_pullAction);

    m_startAction = new QAction("start", this);
    connect(m_startAction, SIGNAL(triggered()), this, SLOT(onStartAction()));
    m_menu->addAction(m_startAction);

    m_stopAction = new QAction("stop", this);
    connect(m_stopAction, SIGNAL(triggered()), this, SLOT(onStopAction()));
    m_menu->addAction(m_stopAction);

    m_deleteAction = new QAction("delete", this);
    connect(m_deleteAction, SIGNAL(triggered()), this, SLOT(onDeleteAction()));
    m_menu->addAction(m_deleteAction);

    onRefresh();
    onRefreshFriend();
}

SkpFileWidget::~SkpFileWidget()
{
    delete ui;
}

QString SkpFileWidget::serverStatus(int status)
{
    if(status == FileDataStatusNull)
        return tr("无效");
    if(status == FileDataStatusPushing)
        return tr("正在上传");
    if(status == FileDataStatusFinish)
        return tr("正常");
}

void SkpFileWidget::onPushFile()
{
    QStringList files = QFileDialog::getOpenFileNames(
                this,
                "Select one or more files to open",
                "",
                tr("*.*"));

    if(files.isEmpty())
        return;
    appendPushFile(files);
}

void SkpFileWidget::appendPushFile(QStringList &files)
{
    for(int i = 0; i < files.size(); i++) {

        //    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        //                                                    "",
        //                                                    tr("*.*"));
        //    if(fileName.isEmpty())
        //        return;

        QString fileName = files.at(i);



        qDebug() << "onPushFile";
        qDebug() << "fileName" << fileName;

        int row = isExistPath(fileName);
        if(row >= 0) {
            selectAndShowRow(row);

            QjtMessageBox::information(this, tr("提示"), tr("文件已经存在"));

            continue;
        }

        SkpTransferFile *transferFile = new SkpTransferFile();
        transferFile->getFileInfors(fileName);
        transferFile->addToFileThread();

        connect(transferFile, SIGNAL(sigReplyMD5(SkpTransferFile*)), this, SLOT(onReplyMD5(SkpTransferFile*)));
        connect(transferFile, SIGNAL(sigPush(SkpTransferFile*,qint32)), this, SLOT(onPush(SkpTransferFile*,qint32)));

        SkpFileData_t *fileData = new SkpFileData_t();
        fileData->path = transferFile->m_path;
        fileData->name = transferFile->m_name;
        fileData->md5 = "";
        fileData->offset = 0;
        fileData->length = transferFile->m_fileSize;
        fileData->serverStatus = FileDataStatusPushing;
        fileData->clientStatus = FileStatusCheckMD5;
        fileData->proportion = 0;
        fileData->rateByte = 0;
        fileData->transferFile = transferFile;

        appendRowData(fileData);

        emit transferFile->emit_GetFileMD5();
    }
}

void SkpFileWidget::onReplyMD5(SkpTransferFile *transferFile)
{
    int row = isExistPath(transferFile->m_path);
    if(row >= 0) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
        fileData->md5 = transferFile->m_md5;
        fileData->clientStatus = FileStatusCheckServer;
        updateRowData(row);
    }

    transferFile->push();
}

void SkpFileWidget::onPush(SkpTransferFile *transferFile, qint32 status)
{
    int row = -1;
    int serverStatus = FileDataStatusPushing;
    if(status == PushRstStatusExist) {
        row = isExist(transferFile->m_path, transferFile->m_md5, FileStatusCheckServer);
    }

    if(status == PushRstStatusRequest ||
            status == PushRstStatusStartPush ||
            status == PushRstStatusPushing ||
            status == PushRstStatusRate ||
            status == PushRstStatusStop ||
            status == PushRstStatusError ||
            status == PushRstStatusSuccess) {
        row = isExistMD5(transferFile->m_md5);
    }

    if(status == PushRstStatusSuccess) {
        serverStatus = FileDataStatusFinish;
    }


    if(row >= 0) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
        fileData->serverStatus = serverStatus;
        fileData->clientStatus = status;
        fileData->offset = transferFile->m_offset;
        fileData->rateByte = transferFile->m_lastRatebytes;
        fileData->proportion = SkpUtility::proportion(transferFile->m_requestDataSize, transferFile->m_fileSize);

        updateRowData(row);
    }

    if(status == PushRstStatusExist) {
        selectAndShowRow(row);
    }


    if(status == PushRstStatusExist ||
            status == PushRstStatusSuccess) {
        transferFile->deleteLater();
    }
}

void SkpFileWidget::onRefresh()
{
    SkpTransferFile *transferFile = new SkpTransferFile();
    connect(transferFile, SIGNAL(sigFileList(SkpTransferFile*,QByteArray)), this, SLOT(onFileList(SkpTransferFile*,QByteArray)));

    transferFile->fileList();
}

void SkpFileWidget::onFileList(SkpTransferFile *transferFile, const QByteArray &data)
{
    transferFile->deleteLater();

    SkpLocalTLV tlvResponse;
    tlvResponse.skp_parse_buffer(data.data(), data.size());

    if(!tlvResponse.skp_status()) {
        MemoryFile::MemoryFileDataList fileDataList;
        tlvResponse.skp_Parse_message(&fileDataList);

        QHash<QString, SkpFileData_t *> hash;
        for(int row = 0; row < ui->tableWidget->rowCount(); row++) {
            SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
            hash.insert(fileData->md5, fileData);
        }

        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        for(int i = 0; i < fileDataList.file_data_size(); i++) {
            MemoryFile::MemoryFileData serverFileData = fileDataList.file_data(i);
            SkpFileData_t *fileData = hash.value(QString::fromStdString(serverFileData.md5()), NULL);
            if(!fileData) {
                fileData = new SkpFileData_t();
                fileData->path = QString::fromStdString(serverFileData.name());
                //fileData->path = QString::fromUtf8(QByteArray(serverFileData.name().c_str(), serverFileData.name().size()));
                QFileInfo fileInfo(fileData->path);
                fileData->name = fileInfo.fileName();;
                fileData->md5 = QString::fromStdString(serverFileData.md5());
                fileData->offset = serverFileData.offset();
                fileData->length = serverFileData.lenth();
                fileData->serverStatus = serverFileData.status();
                fileData->clientStatus = FileStatusNormal;
                fileData->proportion = 0;
                fileData->rateByte = 0;
                fileData->transferFile = 0;
            } else {
                hash.remove(fileData->md5);
            }

            appendRowData(fileData);
        }

        foreach (SkpFileData_t * fileData, hash) {
            appendRowData(fileData);
        }

        hash.clear();
    } else {
        QjtMessageBox::information(this, tr("提示"), tr("刷新失败"));
    }
}

void SkpFileWidget::onPullAction()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty())
        return;

    QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
    int count = ranges.count();
    for(int i = 0; i < count; i++)
    {
        int topRow = ranges.at(i).topRow();
        int bottomRow= ranges.at(i).bottomRow();
        for(int j = topRow; j <= bottomRow; j++)
        {
            pull(j, dir);
        }
    }

}

void SkpFileWidget::onStartAction()
{
    QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
    int count = ranges.count();
    for(int i = 0; i < count; i++)
    {
        int topRow = ranges.at(i).topRow();
        int bottomRow= ranges.at(i).bottomRow();
        for(int j = topRow; j <= bottomRow; j++)
        {
            start(j);
        }
    }
}

void SkpFileWidget::onStopAction()
{
    QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
    int count = ranges.count();
    for(int i = 0; i < count; i++)
    {
        int topRow = ranges.at(i).topRow();
        int bottomRow= ranges.at(i).bottomRow();
        for(int j = topRow; j <= bottomRow; j++)
        {
            stop(j);
        }
    }
}

void SkpFileWidget::pull(int row, QString &dir)
{
    if(row >= 0) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
        if(fileData->serverStatus != FileDataStatusFinish)
            return;
        if(fileData->clientStatus == PullRstStatusPulling)
            return;

        fileData->path = dir + "/" + fileData->name;

        QFileInfo fileInfo(fileData->path);
        if(fileInfo.exists()) {
            QjtMessageBox::information(this, tr("提示"), tr("文件已经存在"));
            return;
        }

        fileData->clientStatus = PullRstStatusPulling;
        fileData->proportion = 0;
        fileData->rateByte = 0;

        SkpTransferFile *transferFile = new SkpTransferFile();
        fileData->transferFile = transferFile;

        connect(transferFile, SIGNAL(sigPull(SkpTransferFile*,QByteArray,qint32)), this, SLOT(onPull(SkpTransferFile*,QByteArray,qint32)));

        transferFile->setPath(fileData->path);
        transferFile->setName(fileData->name);
        transferFile->setMD5(fileData->md5);
        transferFile->setOffset(fileData->offset);
        transferFile->setFileSize(fileData->length);
        transferFile->addToFileThread();
        updateRowData(row);

        transferFile->pull();
    }
}

void SkpFileWidget::start(int row)
{
    if(row >= 0) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
        if(fileData->transferFile->m_type == TransferTypePush)
            fileData->clientStatus = PushRstStatusPushing;
        else
            fileData->clientStatus = PullRstStatusPulling;
        updateRowData(row);
        fileData->transferFile->start();
    }
}

void SkpFileWidget::stop(int row)
{
    if(row >= 0) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
        if(fileData->transferFile->m_type == TransferTypePush)
            fileData->clientStatus = PushRstStatusStop;
        else
            fileData->clientStatus = PullRstStatusStop;
        updateRowData(row);
        fileData->transferFile->stop();
    }

}

void SkpFileWidget::deleteFile(int row)
{
    if(row >= 0) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
        fileData->clientStatus = FileStatusDeleteStart;
        updateRowData(row);

        SkpTransferFile *transferFile = new SkpTransferFile();
        connect(transferFile, SIGNAL(sigDelete(SkpTransferFile*,qint32)), this, SLOT(onDelete(SkpTransferFile*,qint32)));
        transferFile->setMD5(fileData->md5);
        transferFile->deleteFile();
    }
}

void SkpFileWidget::onPull(SkpTransferFile *transferFile, const QByteArray &data, qint32 status)
{
    int row = isExistMD5(transferFile->m_md5);
    if(row >= 0) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);

        fileData->clientStatus = status;
        fileData->rateByte = transferFile->m_lastRatebytes;
        fileData->proportion = SkpUtility::proportion(transferFile->m_requestDataSize, transferFile->m_fileSize);

        updateRowData(row);
    }

    if(status == PullRstStatusPulling ||
            status == PullRstStatusRate ||
            status == PullRstStatusStop ||
            status == PullRstStatusError) {

    }

    if(status == PullRstStatusNULL ||
            status == PullRstStatusSuccess) {
        transferFile->deleteLater();
    }
}

void SkpFileWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    int selectRow = -1;
    if(!ui->tableWidget->selectedItems().isEmpty())
    {
        selectRow = ui->tableWidget->selectedItems().at(0)->row();
    }

    onCellClicked(selectRow, 0);

    if(ui->tableWidget->selectedItems().size() > 0)
        m_menu->exec(QCursor::pos());
}

void SkpFileWidget::onCellEntered(int row, int column)
{
    if(m_currMouseRow >= 0) {
        setRowColor(m_currMouseRow, m_defaultBackgroundColor);
        m_currMouseRow = -1;
    }


    int selectRow = -1;
    if(!ui->tableWidget->selectedItems().isEmpty())
    {
        selectRow = ui->tableWidget->selectedItems().at(0)->row();
    }

    if(selectRow == row)
        return;


    m_currMouseRow = row;
    //m_defaultBackgroundColor = ui->tableWidget->item(row, column)->backgroundColor();
    setRowColor(row, m_mouseMoveColor);

}

void SkpFileWidget::onCellClicked(int row, int column)
{
    if(m_currMouseRow >= 0) {
        setRowColor(m_currMouseRow, m_defaultBackgroundColor);
        m_currMouseRow = -1;
    }

    m_currMouseRow = row;
    //m_defaultBackgroundColor = ui->tableWidget->item(row, column)->backgroundColor();
}

void SkpFileWidget::setRowColor(int row, QColor &color)
{
    for(int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        QTableWidgetItem *item = ui->tableWidget->item(row, i);
        item->setBackgroundColor(color);
    }
}

int SkpFileWidget::isExist(QString &path, QString &md5, qint32 clientStatus)
{
    for(int row = 0; row < ui->tableWidget->rowCount(); row++) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
        if(path == fileData->path && md5 == fileData->md5 && clientStatus == fileData->clientStatus) {
            return row;
        }
    }

    return -1;
}

int SkpFileWidget::isExistFileToFriend(QString &name, QString &md5, quint64 friendCode)
{
    for(int row = 0; row < ui->tableWidget->rowCount(); row++) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
        if(name == fileData->name && md5 == fileData->md5 && friendCode == fileData->friendCode) {
            return row;
        }
    }

    return -1;
}

int SkpFileWidget::isExistMD5(QString &md5)
{
    for(int row = 0; row < ui->tableWidget->rowCount(); row++) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
        if(md5 == fileData->md5) {
            return row;
        }
    }

    return -1;
}

int SkpFileWidget::isExistPath(QString &path)
{
    qDebug() << "path" << path << path.size() << "xxx";
    for(int row = 0; row < ui->tableWidget->rowCount(); row++) {
        SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
        qDebug() << "fileData->path" << fileData->path << fileData->path.size() << "xxx";
        if(path == fileData->path) {
            return row;
        }
    }

    return -1;
}

void SkpFileWidget::selectAndShowRow(int row)
{
    int min = ui->tableWidget->verticalScrollBar()->minimum();
    int max = ui->tableWidget->verticalScrollBar()->maximum();
    double rowHeight = (double)(max - min) / ui->tableWidget->rowCount();
    int curr = min + (row * rowHeight);
    ui->tableWidget->verticalScrollBar()->setValue(curr);
    ui->tableWidget->setCurrentCell(row , QItemSelectionModel::Select);
}

void SkpFileWidget::appendRowData(void *data)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    insertRowData(row, data);
}

void SkpFileWidget::insertRowData(int row, void *data)
{
    for(int column = 0; column < ui->tableWidget->columnCount(); column++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        ui->tableWidget->setItem(row, column, item);
    }

    ui->tableWidget->item(row, TableHeaderTypeName)->setData(Qt::UserRole, (quint32)data);

    updateRowData(row);
}

void *SkpFileWidget::getRowData(int row)
{
    quint32 addr = ui->tableWidget->item(row, TableHeaderTypeName)->data(Qt::UserRole).toUInt();
    return (void *)addr;
}

void SkpFileWidget::updateRowData(int row)
{

    SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
    QString proportion = fileData->proportion ? QString::number(fileData->proportion) + "%" : "";
    QString rateByte = fileData->rateByte? SkpUtility::getRateByteStr(fileData->rateByte) : "";

    QStringList list;
    list << fileData->name
         << fileData->md5
         << (fileData->offset?QString::number(fileData->offset):"")
         << SkpUtility::getRateByteStr(fileData->length)
         << serverStatus(fileData->serverStatus)
         << clientStatus(fileData->clientStatus)
         << ((proportion.isEmpty() && rateByte.isEmpty()) ? "" : ( proportion + " | " + rateByte));

    for(int column = 0; column < ui->tableWidget->columnCount(); column++) {
        QTableWidgetItem *item = ui->tableWidget->item(row, column);
        if(column > TableHeaderTypeName)
            item->setTextAlignment(Qt::AlignCenter);
        item->setText(list.at(column));
    }
}

void SkpFileWidget::onDeleteAction()
{
    QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
    int count = ranges.count();
    for(int i = 0; i < count; i++)
    {
        int topRow = ranges.at(i).topRow();
        int bottomRow= ranges.at(i).bottomRow();
        for(int j = topRow; j <= bottomRow; j++)
        {
            deleteFile(j);
        }
    }
}

void SkpFileWidget::onDelete(SkpTransferFile *transferFile, qint32 status)
{
    int row = isExistMD5(transferFile->m_md5);
    if(row >= 0) {
        if(status == DeleteStatusError) {
            SkpFileData_t *fileData = (SkpFileData_t *)getRowData(row);
            fileData->clientStatus = DeleteStatusError;
            updateRowData(row);
            ui->tableWidget->removeRow(row);
            transferFile->deleteLater();
        } else {
            ui->tableWidget->removeRow(row);
            transferFile->deleteLater();
        }

        if(row == ui->tableWidget->rowCount())
            m_currMouseRow = ui->tableWidget->rowCount() - 1;
    }
}

void SkpFileWidget::onTtemDoubleClicked(QTableWidgetItem *item)
{
    QString friendCodeStr = ui->friend_tableWidget->item(item->row(), 0)->text().trimmed();
    QString friendNameStr = ui->friend_tableWidget->item(item->row(), 1)->text().trimmed();


    SkpChatWidget *chatWidget = friendChat(friendCodeStr, friendNameStr);\
    chatWidget->show();
}

void SkpFileWidget::onRefreshFriend()
{
    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalLoginFriendList);

    m_signal->emit_sigKeep([this](const QByteArray &bytes)
    {
        emit sigRefreshFriend(bytes);
    });
}

void SkpFileWidget::onRefreshFriend(const QByteArray &data)
{
    SkpLocalTLV tlvResponse;
    tlvResponse.skp_parse_buffer(data.data(), data.size());

    if(tlvResponse.skp_status()) {

    } else {
        ui->friend_tableWidget->clearContents();
        ui->friend_tableWidget->setRowCount(0);
        Friend::FriendList friendList;
        tlvResponse.skp_Parse_message(&friendList);
        for(int i = 0; i < friendList.friend__size(); i++) {
            Friend::Friend friendData = friendList.friend_(i);
            if(friendData.friend_() == SkpSocketManage::m_loginCode)
                continue;
            int row = ui->friend_tableWidget->rowCount();
            ui->friend_tableWidget->insertRow(row);
            for(int column = 0; column < ui->friend_tableWidget->columnCount(); column++) {
                QTableWidgetItem *item = new QTableWidgetItem();
                ui->friend_tableWidget->setItem(row, column, item);
            }

            QStringList list;
            list << QString::number(friendData.friend_())
                 << QString::fromStdString(friendData.name())
                 << QString::fromStdString(friendData.password());

            for(int column = 0; column < ui->friend_tableWidget->columnCount(); column++) {
                QTableWidgetItem *item = ui->friend_tableWidget->item(row, column);
                item->setTextAlignment(Qt::AlignCenter);
                item->setText(list.at(column));
            }

        }

    }
}

void SkpFileWidget::registerCallbackChat(QByteArray &bytes)
{
    emit sigFriendChat(bytes);
}

void SkpFileWidget::onFriendChat(const QByteArray &data)
{
    Ack::MessageAck ack;
    ack.ParseFromArray(data.data(), data.size());

    QString friendCodeAck = QString::number(ack.friend_());

    SkpChatWidget *chatWidget = friendChat(friendCodeAck);
    chatWidget->m_chatMainWidget->setAck(ack);
}

SkpChatWidget *SkpFileWidget::setFriendChat(QString &friendcode, QString &friendName)
{
    SkpChatWidget *chatWidget = new SkpChatWidget();
    chatWidget->m_chatMainWidget->setData(friendcode, friendName);
    m_friendChatHash.insert(friendcode.toULongLong(), chatWidget);
    return chatWidget;
}

SkpChatWidget *SkpFileWidget::friendChat(QString &friendcode, QString &friendName)
{
   SkpChatWidget *chatWidget = m_friendChatHash.value(friendcode.toULongLong());
   if(chatWidget)
       return chatWidget;
   return setFriendChat(friendcode, friendName);
}

SkpChatWidget *SkpFileWidget::friendChat(QString &friendcode)
{
    QString friendNameStr;
    for(int row = 0; row < ui->friend_tableWidget->rowCount(); row++) {
        QString friendCodeStr = ui->friend_tableWidget->item(row, 0)->text().trimmed();
        if(friendcode == friendCodeStr) {
            friendNameStr = ui->friend_tableWidget->item(row, 1)->text().trimmed();
            break;
        }
    }

    return friendChat(friendcode, friendNameStr);
}


