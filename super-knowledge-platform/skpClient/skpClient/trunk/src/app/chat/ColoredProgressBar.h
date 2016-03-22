#ifndef COLOREDPROGRESSBAR_H
#define COLOREDPROGRESSBAR_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>

enum
{
    ACTION_STATUS_QUEUE,
    ACTION_STATUS_UPLOADING,
    ACTION_STATUS_PAUSE
};

class ColoredProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit ColoredProgressBar(QWidget *parent = 0);
    ~ColoredProgressBar();

    void paintEvent(QPaintEvent *event);

    inline qint64 value() { return m_value; }

public slots:
    void setRange(qint64 minimum, qint64 maximum);
    void setValue(qint64 value, qint64 speed);
    void setValueMaximum(qint64 value, qint64 maximum);
    //void onSpeed(double speedByS);
    void onUpload_status(int status);

protected:

    QTimer m_timer;

    qint64 m_value;

    qint64 m_minimum;
    qint64 m_maximum;

    qint64 m_currentSpeed;

    QPixmap m_uploadStatusPixmap;

private slots:
    void onTimeout();
};

class ColoredProgressBarEx : public ColoredProgressBar
{
    Q_OBJECT
public:
    explicit ColoredProgressBarEx(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);

    bool m_bIsStarted;
};

#endif // COLOREDPROGRESSBAR_H
