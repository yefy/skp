#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QLocale>

#include "ColoredProgressBar.h"


QString formatSize(qint64 bytes)
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

ColoredProgressBar::ColoredProgressBar(QWidget *parent) :
    QWidget(parent)
  , m_value(0)
  , m_minimum(0)
  , m_maximum(100)
  , m_currentSpeed(0)
{
    m_timer.setSingleShot(false);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    m_uploadStatusPixmap.load(":/theme/def/queued.png");
}

ColoredProgressBar::~ColoredProgressBar()
{
}

void ColoredProgressBar::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();

    r.setWidth(r.width() - 1);
    r.setHeight(r.height() - 1);

    QPen boundPen(QColor(90, 140, 90, 150));
    p.setPen(boundPen);

    QPainterPath boundPath;
    boundPath.addRoundedRect(r, 5, 5);

    p.drawPath(boundPath);

    QLinearGradient linearGradient(r.x(), r.y(), r.width(), r.height());
    linearGradient.setColorAt(0, QColor(156, 255, 248, 255));
    linearGradient.setColorAt(0.55, QColor(65, 87, 222, 255));
    linearGradient.setColorAt(0.77, QColor(50, 57, 208, 255));
    linearGradient.setColorAt(1, QColor(163, 55, 252, 255));

    double value_minimum = m_value - m_minimum;
    double maximum_minimum = m_maximum - m_minimum;

    int ratioWidth = value_minimum / maximum_minimum * r.width();
    QRect r_ratio = r;
    r_ratio.setWidth(ratioWidth);

    QPainterPath ratioPath;
    ratioPath.addRoundedRect(r_ratio, 5, 5);

    p.setBrush(linearGradient);
    p.fillPath(ratioPath, QBrush(linearGradient));
    //p.drawRect(r);

    // 绘制进度文本
    double textValue = value_minimum * 100.00 / maximum_minimum ;
    QString text = QString().sprintf("%.2lf%%", textValue);

    QTextOption textOption;
    textOption.setAlignment(Qt::AlignHCenter);

    QRect textRect(0,0, fontMetrics().width(text), fontMetrics().height());
    textRect.moveCenter(r.center());

    p.setPen(QPen(Qt::red));
    p.drawText(textRect, text, textOption);

    QWidget::paintEvent(event);
}

void ColoredProgressBar::setRange(qint64 minimum, qint64 maximum)
{
    qint64 min = minimum;
    qint64 max = maximum;

    if(min < 0)
        min = 0;

    if(max < min)
        max = min;

    m_minimum = min;
    m_maximum = max;

    update();
}

void ColoredProgressBar::setValue(qint64 value, qint64 speed)
{
    qint64 v = value;

    if(v < m_minimum)
        v = m_minimum;

    if(v > m_maximum)
        v = m_maximum;

    if(v )

    m_value = v;
    m_currentSpeed = speed;

    update();
}

void ColoredProgressBar::setValueMaximum(qint64 value, qint64 maximum)
{
    m_minimum = 0;
    m_value = value;
    m_maximum = maximum;

    update();
}
/*
#include <QDebug>
void ColoredProgressBar::onSpeed(double speedByS)
{
    double speed = speedByS;
    static qint32 unit = 1024;
    static qint32 unint2 = 1048576; // 1024 *1024
    static qint32 unint3 =  1073741824; // 1024 * 1024 *1024

    if(speed < unit)
    {
        m_currentSpeed = QString().sprintf("%.1lf", speed) + "B/s";
    } else if(speed >= unit && speed < unint2)
    {
        m_currentSpeed = QString().sprintf("%.1lf", speed / unit) + "KB/s";
    } else if(speed > unint2 && speed < unint3)
    {
        m_currentSpeed = QString().sprintf("%.2lf", speed / unint2) + "MB/s";
    }

    qDebug() << m_currentSpeed << "***********************";
}
*/

void ColoredProgressBar::onUpload_status(int status)
{
    switch(status)
    {
    case ACTION_STATUS_QUEUE:
        m_uploadStatusPixmap.load(":/theme/def/queued.png");
        break;
    case ACTION_STATUS_PAUSE:
        m_uploadStatusPixmap.load(":/theme/def/paused.png");
        break;
    case ACTION_STATUS_UPLOADING:
    default:
        m_uploadStatusPixmap.load(":/theme/def/uploading.png");
        break;
    }
}

void ColoredProgressBar::onTimeout()
{
    ++m_value;
    if(m_value == 101)
        m_value = 0;

    update();
}

ColoredProgressBarEx::ColoredProgressBarEx(QWidget *parent)
    : ColoredProgressBar(parent)
    , m_bIsStarted(false)
{
}

void ColoredProgressBarEx::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int pixmapwidth = 20;

    QRect pixmapRect(0, 0, 10, 10);
    pixmapRect.moveCenter(QRect(0, 0, pixmapwidth, this->height()).center());
    p.drawPixmap(pixmapRect, m_uploadStatusPixmap);

    p.translate(pixmapwidth, 0);

    QRect r = rect();

    int r_width = this->width() - pixmapwidth - 5;
    int r_height = this->height() / 3;
    int t_height = this->height() - r_height;

    r.setX(0);
    r.setY(0);
    r.setWidth(r_width);
    r.setHeight(r_height);

    QPen boundPen(QColor(90, 140, 90, 150));
    //p.setPen(Qt::NoPen);
    p.setPen(boundPen);

    QPainterPath boundPath;
    boundPath.addRoundedRect(r, 5, 5);

    p.drawPath(boundPath);

    QLinearGradient linearGradient(r.x(), r.y(), r.width(), r.height());
    linearGradient.setColorAt(0, QColor(156, 255, 248, 255));
    linearGradient.setColorAt(0.55, QColor(65, 87, 222, 255));
    linearGradient.setColorAt(0.77, QColor(50, 57, 208, 255));
    linearGradient.setColorAt(1, QColor(163, 55, 252, 255));

    double value_minimum = m_value - m_minimum;
    double maximum_minimum = m_maximum - m_minimum;

    int ratioWidth = value_minimum / maximum_minimum * r.width();
    QRect r_ratio = r;
    r_ratio.setWidth(ratioWidth);

    QPainterPath ratioPath;
    ratioPath.addRoundedRect(r_ratio, 5, 5);

    p.setBrush(linearGradient);
    p.fillPath(ratioPath, QBrush(linearGradient));
    //p.drawRect(r);

    // 绘制进度文本
//    double textValue = value_minimum * 100.00 / maximum_minimum ;
//    QString text = QString().sprintf("%.2lf%%", textValue);

//    QTextOption textOption;
//    textOption.setAlignment(Qt::AlignHCenter);

//    QRect textRect(0,0, fontMetrics().width(text), fontMetrics().height());
//    textRect.moveCenter(r.center());

//    p.setPen(QPen(Qt::red));
//    p.drawText(textRect, text, textOption);

    // 绘制已传输大小和总大小
    //p.translate(r_width + 10, r_height);

    QRect t = rect();
    t.setX(0);
    t.setY(r_height);
    t.setWidth(r_width);
    t.setHeight(t_height);

    QString infoText = formatSize(m_value) + "/" + formatSize(m_maximum) + " - " + formatSize(m_currentSpeed);
    p.setPen(QPen(Qt::red));
    p.drawText(t, infoText);
}
