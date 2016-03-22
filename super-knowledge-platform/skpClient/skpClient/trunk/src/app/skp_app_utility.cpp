#include "skp_app_utility.h"
#include <QByteArray>
#include <QFile>
#include <QCryptographicHash>
#include <QDebug>
#include <QTime>
#include <time.h>
#include <QString>
#include "skp_core_md5.h"
#include <QFileInfo>

#define USER_QT_MD5 1

SkpUtility::SkpUtility()
{
}

quint64 SkpUtility::skp_get_file_md5(const QString &filePath, QString &md5)
{
#if USER_QT_MD5
    QFile localFile(filePath);

    if (!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error.";
        return 0;
    }

    QCryptographicHash ch(QCryptographicHash::Md5);

    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 128;
    QByteArray buf;

    totalBytes = localFile.size();
    bytesToWrite = totalBytes;

    while (1)
    {
        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }

        if(bytesWritten == totalBytes)
        {
            break;
        }
    }

    localFile.close();
    md5 = ch.result().toHex();
    return totalBytes;
#else

    qDebug() << "size 111" << filePath.size();
    qDebug() << "size 222" << filePath.toLatin1().size();
    qDebug() << "size 333" << filePath.toLocal8Bit().size();
    qDebug() << "size 444" << filePath.toStdString().size();

    char *md5_c = MDFile(filePath.toLocal8Bit().data());
    md5 = QString(md5_c);
    QFileInfo fileInfo(filePath);
    QString name = fileInfo.fileName();
    qDebug() << "size 111" << name.size();
    qDebug() << "size 222" << name.toLatin1().size();
    qDebug() << "size 333" << name.toLocal8Bit().size();
    qDebug() << "size 444" << name.toStdString().size();
    qDebug() << "size 555" << name.toUtf8().size();
    return fileInfo.size();
#endif

}

QString SkpUtility::skp_get_data_md5(const QString &data)
{
#if USER_QT_MD5
    QByteArray byte = QCryptographicHash::hash(data.toLatin1(),QCryptographicHash::Md5 );
    return byte.toHex();
#else
    char *md5 = MDString(data.toStdString().c_str(), data.toStdString().size());
    return QString(md5);
#endif
}


QString SkpUtility::getRateByteStr(qint64 bytes)
{
    /// According to the Si standard KB is 1000 bytes, KiB is 1024
    /// but on windows sizes are calculated by dividing by 1024 so we do what they do.
    const qint64 kb = 1024;
    const qint64 mb = 1024 * kb;
    const qint64 gb = 1024 * mb;
    const qint64 tb = 1024 * gb;

    if (bytes >= tb)
        return QString(QObject::tr("%1 TB")).arg(QLocale().toString(qreal(bytes) / tb, 'f', 3));

    if (bytes >= gb)
        return QString(QObject::tr("%1 GB")).arg(QLocale().toString(qreal(bytes) / gb, 'f', 2));

    if (bytes >= mb)
        return QString(QObject::tr("%1 MB")).arg(QLocale().toString(qreal(bytes) / mb, 'f', 1));

    if (bytes >= kb)
        return QString(QObject::tr("%1 KB")).arg(QLocale().toString(bytes / kb));

    return QString(QObject::tr("%1 bytes")).arg(QLocale().toString(bytes));
}

double SkpUtility::proportion(double currSize, double allSize)
{
    return (currSize / allSize * 100);
}
