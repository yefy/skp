#include <QApplication>
#include <QTranslator>
#include <QFontDatabase>
#include <QString>
#include <QDebug>

#include "skp_mainwindow.h"
#include "skp_themed_base.h"
#include "skp_login_widget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    if(translator.load(QApplication::applicationDirPath()+"/zh_CN.qm"))
            QApplication::installTranslator(&translator);

    QString fontPath = QApplication::applicationDirPath() + "/font/msyh.ttf";

    int fontId = QFontDatabase::addApplicationFont(fontPath);
    if(fontId != -1)
    {
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).first();
        QFont font(fontFamily, 9);
        a.setFont(font);
    }



    SkpLoginWidget login;
    login.show();

    skp_update_Style_Sheet();

    return a.exec();
}
