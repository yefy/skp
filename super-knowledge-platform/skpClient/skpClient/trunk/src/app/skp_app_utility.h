#ifndef SKP_APP_UTILITY_H
#define SKP_APP_UTILITY_H

#include <QString>

class SkpUtility
{
public:
    SkpUtility();
    static quint64 skp_get_file_md5(const QString &filePath, QString &md5);
    static QString skp_get_data_md5(const QString &data);
    static QString getRateByteStr(qint64 bytes);
    static double proportion(double currSize, double allSize);
};

#endif // SKP_APP_UTILITY_H
