#include "skp_themed_base.h"
#include <QPainter>
#include <QWidget>
#include "skp_utility.h"
#include "MOption.h"
#include <QCoreApplication>
#include <QApplication>
#include <QFile>
#include <QPainterPath>
#include <QtMath>
#include <QDebug>

static QList<SkpThemedBase *> m_themedList;

SkpThemedBase::SkpThemedBase(QWidget *parent) :
    m_parent(parent),
    m_titleHeight(0),
    m_linearHeight(0),
    m_statusHeight(0)
{
    skp_register_themed();
    skp_apply_themed();
}

SkpThemedBase::~SkpThemedBase()
{
    skp_remove_themed();
}

void SkpThemedBase::skp_register_themed()
{
    m_themedList.append(this);
}

void SkpThemedBase::skp_remove_themed()
{
    m_themedList.removeAll(this);
}

void SkpThemedBase::skp_set_Paint_Height(int title, int linear, int status)
{
    m_titleHeight = title;
    m_linearHeight = linear;
    m_statusHeight = status;
}

void SkpThemedBase::skp_apply_themed()
{
    m_aeroTransparent = MOption::instance()->option("AeroTransparent", "theme").toInt();
    m_widgetTransparent = MOption::instance()->option("WidgetTransparent", "theme").toInt();

    m_cachedPixmap = QPixmap(QCoreApplication::applicationDirPath() + MOption::instance()->option("WindowBGPixmap", "theme").toString());
    m_cachedPixmap = setAlphaPixmap(m_cachedPixmap, m_aeroTransparent);

    m_cachedColor = MOption::instance()->option("WindowBGColor", "theme").value<QColor>();
    m_cachedColor.setAlpha(m_aeroTransparent);

    m_parent->update();
}

void SkpThemedBase::skp_draw_themed()
{
//    QPainterPath path;
//    path.setFillRule(Qt::WindingFill);
//    path.addRect(10, 10, m_parent->width()-20, m_parent->height()-20);

//    QPainter painter(m_parent);
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.fillPath(path, QBrush(Qt::white));

//    QColor color(0, 0, 0, 50);
//    for(int i=0; i<10; i++)
//    {
//        QPainterPath path;
//        path.setFillRule(Qt::WindingFill);
//        path.addRect(10-i, 10-i, m_parent->width()-(10-i)*2, m_parent->height()-(10-i)*2);
//        color.setAlpha(150 - qSqrt(i)*50);
//        painter.setPen(color);
//        painter.drawPath(path);
//    }



        /*
    QPainter p(m_parent);

    QPen pen(QColor(255, 255, 255, 255));
    pen.setWidth(2);
    pen.setStyle(Qt::NoPen);
    p.setPen(pen);

    QPainterPath path;
    QPoint bottomRight = m_parent->rect().bottomRight();
    QRect pathRect = m_parent->rect();
    pathRect.setBottomRight(QPoint(bottomRight.x()-1, bottomRight.y()-1));
    path.addRoundedRect(pathRect, 0, 0);




    QString themeType = MOption::instance()->option("WindowBGPixmapType", "theme").toString();
    if(themeType == "bitmap") {
        if(!m_cachedPixmap.isNull()) {
            p.fillPath(path, QBrush(m_cachedPixmap));
        } else {
            QColor color(100,178,226,255);
            MOption::instance()->setOption(QVariant(color), "WindowBGColor", "theme");
            MOption::instance()->setOption("color", "WindowBGPixmapType", "theme");
            MOption::instance()->setOption(QVariant(color), OPTION_AVERAGE_COLOR, OPTION_GROUP_Theme);

            p.fillPath(path, color);
        }
    } else if(themeType == "color") {
        QImage image(QSize(100, 100), QImage::Format_ARGB32);
        image.fill(MOption::instance()->option("WindowBGColor", "theme").value<QColor>());
        m_cachedPixmap = setAlphaPixmap(QPixmap::fromImage(image), m_aeroTransparent);
        p.fillPath(path, QBrush(m_cachedPixmap));
    }

    QRect linearRect(0, m_titleHeight, m_parent->width(), m_linearHeight);
    QLinearGradient linear(0, m_titleHeight, 0, m_linearHeight+m_titleHeight);

    QColor c1(255, 255, 255, m_widgetTransparent /2);;
    QColor c2(255, 255, 255, m_widgetTransparent);

    linear.setColorAt(0.0, QColor(255, 255, 255, 255));
    linear.setColorAt(0.5, c1);
    linear.setColorAt(1.0, c2);

    p.setBrush(QBrush(linear));
    p.fillRect(linearRect, QBrush(linear));

    QPainterPath clientPath;
    QRect clientRect(0, m_titleHeight + m_linearHeight,
                     m_parent->width(), m_parent->height() - m_titleHeight - m_linearHeight - m_statusHeight);
    clientPath.addRect(clientRect);
    p.fillPath(clientPath, QBrush(QColor(255, 255, 255, m_widgetTransparent)));

    p.strokePath(path, QPen(QColor(0, 0, 0, 150)));
    */


    QPainter p(m_parent);

    int width = 5;
    QColor color(0, 0, 0, 20);
    for(int i=0; i<width; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(width-i, width-i, m_parent->width()-(width-i)*2, m_parent->height()-(width-i)*2);

        color.setAlpha(60 - qSqrt(i)*30);
        p.setPen(color);
        p.drawPath(path);
    }

    QRect pathRect(width, width, m_parent->rect().width() - width * 2, m_parent->rect().height() - width * 2);
    QPainterPath path;
    path.addRoundedRect(pathRect, 0, 0);



    QString themeType = MOption::instance()->option("WindowBGPixmapType", "theme").toString();
    if(themeType == "bitmap") {
        if(!m_cachedPixmap.isNull()) {
            p.fillPath(path, QBrush(m_cachedPixmap));
        } else {
            QColor color(100,178,226,255);
            MOption::instance()->setOption(QVariant(color), "WindowBGColor", "theme");
            MOption::instance()->setOption("color", "WindowBGPixmapType", "theme");
            MOption::instance()->setOption(QVariant(color), OPTION_AVERAGE_COLOR, OPTION_GROUP_Theme);

            p.fillPath(path, color);
        }
    } else if(themeType == "color") {
        QImage image(QSize(100, 100), QImage::Format_ARGB32);
        image.fill(MOption::instance()->option("WindowBGColor", "theme").value<QColor>());
        m_cachedPixmap = setAlphaPixmap(QPixmap::fromImage(image), m_aeroTransparent);
        p.fillPath(path, QBrush(m_cachedPixmap));
    }

    QRect linearRect(0, m_titleHeight, m_parent->width(), m_linearHeight);
    QLinearGradient linear(0, m_titleHeight, 0, m_linearHeight+m_titleHeight);

    QColor c1(255, 255, 255, m_widgetTransparent /2);;
    QColor c2(255, 255, 255, m_widgetTransparent);

    linear.setColorAt(0.0, QColor(255, 255, 255, 255));
    linear.setColorAt(0.5, c1);
    linear.setColorAt(1.0, c2);

    p.setBrush(QBrush(linear));
    p.fillRect(linearRect, QBrush(linear));

    QPainterPath clientPath;
    QRect clientRect(width, m_titleHeight + m_linearHeight,
                     m_parent->width() - width * 2, m_parent->height() - m_titleHeight - m_linearHeight - m_statusHeight - width * 2);
    clientPath.addRect(clientRect);
    p.fillPath(clientPath, QBrush(QColor(255, 255, 255, m_widgetTransparent)));

}

void SkpThemedBase::skp_draw_themed(QPainter &p)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, m_parent->width()-20, m_parent->height()-20);

    QPainter painter(m_parent);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for(int i=0; i<10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10-i, 10-i, m_parent->width()-(10-i)*2, m_parent->height()-(10-i)*2);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }


    /*
    QPen pen(QColor(255, 255, 255, 255));
    pen.setWidth(2);
    pen.setStyle(Qt::NoPen);
    p.setPen(pen);

    QPainterPath path;
    QPoint bottomRight = m_parent->rect().bottomRight();
    QRect pathRect = m_parent->rect();
    pathRect.setBottomRight(QPoint(bottomRight.x()-1, bottomRight.y()-1));
    path.addRoundedRect(pathRect, 0, 0);

    QString themeType = MOption::instance()->option("WindowBGPixmapType", "theme").toString();
    if(themeType == "bitmap")
    {
        if(!m_cachedPixmap.isNull())
        {
            p.fillPath(path, QBrush(m_cachedPixmap));
        }
        else
        {
            QColor color(100,178,226,255);
            MOption::instance()->setOption(QVariant(color), "WindowBGColor", "theme");
            MOption::instance()->setOption("color", "WindowBGPixmapType", "theme");
            MOption::instance()->setOption(QVariant(color), OPTION_AVERAGE_COLOR, OPTION_GROUP_Theme);

            p.fillPath(path, color);
        }
    } else if(themeType == "color")
    {
        QImage image(QSize(100, 100), QImage::Format_ARGB32);
        image.fill(MOption::instance()->option("WindowBGColor", "theme").value<QColor>());
        m_cachedPixmap = setAlphaPixmap(QPixmap::fromImage(image), m_aeroTransparent);
        p.fillPath(path, QBrush(m_cachedPixmap));
    }

    QRect linearRect(0, m_titleHeight, m_parent->width(), m_linearHeight);
    QLinearGradient linear(0, m_titleHeight, 0, m_linearHeight+m_titleHeight);

    QColor c1(255, 255, 255, m_widgetTransparent /2);;
    QColor c2(255, 255, 255, m_widgetTransparent);

    linear.setColorAt(0.0, QColor(255, 255, 255, 0));
    linear.setColorAt(0.5, c1);
    linear.setColorAt(1.0, c2);

    p.setBrush(QBrush(linear));
    p.fillRect(linearRect, QBrush(linear));

    QPainterPath clientPath;
    QRect clientRect(0, m_titleHeight + m_linearHeight,
                     m_parent->width(), m_parent->height() - m_titleHeight - m_linearHeight - m_statusHeight);
    clientPath.addRect(clientRect);
    p.fillPath(clientPath, QBrush(QColor(255, 255, 255, m_widgetTransparent)));

    p.strokePath(path, QPen(QColor(0, 0, 0, 150)));
    */
}

void skp_updata_themed()
{
    foreach (SkpThemedBase *w, m_themedList) {
        w->skp_apply_themed();
    }
}


void skp_update_Style_Sheet()
{
    QColor c = MOption::instance()->option(OPTION_AVERAGE_COLOR,
                                             OPTION_GROUP_Theme).value<QColor>();


    //int widgetTransparent = MOption::instance()->option(OPTION_WidgetTransparent, OPTION_GROUP_Theme).toInt();
    int aeroTransparent = MOption::instance()->option(OPTION_AeroTransparent, OPTION_GROUP_Theme).toInt();
    QString sheetPath = QApplication::applicationDirPath() + "/stylesheet";
    // 读取所有的style sheet
    QStringList styleSheetList;
    styleSheetList << sheetPath + "/default.qss"
                   << sheetPath + "/hbar.qss"
                   << sheetPath + "/menu.qss"
                   << sheetPath + "/tabview.qss"
                   << sheetPath + "/vbar.qss"
                   << sheetPath + "/checkbox.qss";

    QString sheetStr;
    for(int i = 0; i < styleSheetList.size(); ++i)
    {
        const QString &str = styleSheetList.at(i);

        QFile file(str);
        if(file.open(QIODevice::ReadOnly))
        {
            sheetStr += file.readAll();
        }
    }

    sheetStr.replace("menu_background-color_red",   QString::number(c.red()));
    sheetStr.replace("menu_background-color_green", QString::number(c.green()));
    sheetStr.replace("menu_background-color_blue",  QString::number(c.blue()));
    sheetStr.replace("menu_background-color_alpha", QString::number(aeroTransparent));

    sheetStr.replace("horizontal_handle_1", QString::number(c.red()));
    sheetStr.replace("horizontal_handle_2", QString::number(c.green()));
    sheetStr.replace("horizontal_handle_3", QString::number(c.blue()));
    sheetStr.replace("horizontal_handle_4", QString::number(60));

    sheetStr.replace("horizontal_handle_hover_1", QString::number(c.red()));
    sheetStr.replace("horizontal_handle_hover_2", QString::number(c.green()));
    sheetStr.replace("horizontal_handle_hover_3", QString::number(c.blue()));
    sheetStr.replace("horizontal_handle_hover_4", QString::number(100));

    sheetStr.replace("horizontal_sub-line_1", QString::number(c.red()));
    sheetStr.replace("horizontal_sub-line_2", QString::number(c.green()));
    sheetStr.replace("horizontal_sub-line_3", QString::number(c.blue()));
    sheetStr.replace("horizontal_sub-line_4", QString::number(140));

    sheetStr.replace("horizontal_add-line_1", QString::number(c.red()));
    sheetStr.replace("horizontal_add-line_2", QString::number(c.green()));
    sheetStr.replace("horizontal_add-line_3", QString::number(c.blue()));
    sheetStr.replace("horizontal_add-line_4", QString::number(140));

    sheetStr.replace("vertical_handle_1", QString::number(c.red()));
    sheetStr.replace("vertical_handle_2", QString::number(c.green()));
    sheetStr.replace("vertical_handle_3", QString::number(c.blue()));
    sheetStr.replace("vertical_handle_4", QString::number(60));

    sheetStr.replace("vertical_handle_hover_1", QString::number(c.red()));
    sheetStr.replace("vertical_handle_hover_2", QString::number(c.green()));
    sheetStr.replace("vertical_handle_hover_3", QString::number(c.blue()));
    sheetStr.replace("vertical_handle_hover_4", QString::number(100));

    sheetStr.replace("vertical_sub-line_1", QString::number(c.red()));
    sheetStr.replace("vertical_sub-line_2", QString::number(c.green()));
    sheetStr.replace("vertical_sub-line_3", QString::number(c.blue()));
    sheetStr.replace("vertical_sub-line_4", QString::number(140));

    sheetStr.replace("vertical_add-line_1", QString::number(c.red()));
    sheetStr.replace("vertical_add-line_2", QString::number(c.green()));
    sheetStr.replace("vertical_add-line_3", QString::number(c.blue()));
    sheetStr.replace("vertical_add-line_4", QString::number(140));

    sheetStr.replace("QTableView_background_color_red",     QString::number(c.red()));
    sheetStr.replace("QTableView_background_color_green",   QString::number(c.green()));
    sheetStr.replace("QTableView_background_color_blue",    QString::number(c.blue()));
    sheetStr.replace("QTableView_background_color_alpha",   QString::number(aeroTransparent));

    qApp->setStyleSheet(sheetStr);

    skp_updata_themed();
}
