#include "skp_mainwindow.h"
#include "ui_skp_mainwindow.h"
#include "NcFramelessHelper.h"
#include "skp_socket_test_widget.h"
#include "skp_skin_themed_widget.h"
#include "skp_file_widget.h"
#include "MOption.h"

#ifdef Q_OS_WIN
#include <qwinfunctions.h>
#endif

#include <QSystemTrayIcon>
#include <QMenu>
#include <QLabel>
#include <QPainter>


SkpMainWindow::SkpMainWindow(QWidget *parent) :
    QMainWindow(parent),
    SkpChangeWidget(this),
    SkpThemedBase(this),
    ui(new Ui::SkpMainWindow),
    m_framelessHelper(NULL),
    m_socketTestWidget(NULL)
{
    ui->setupUi(this);

    m_framelessHelper = new NcFramelessHelper;
    m_framelessHelper->setWidgetMovable(true);
    m_framelessHelper->setWidgetResizable(true);
    m_framelessHelper->activateOn(this);

    skp_set_move(m_framelessHelper);


    ui->centralWidget->setContentsMargins(5,5,5,5);


#ifdef Q_OS_WIN
    //QtWin::enableBlurBehindWindow(this);
#endif
    setWindowTitle(tr("SKP"));
    ui->titleWidget->setTitle(tr("SKP"));
    ui->titleWidget->setFixedHeight(30);
    ui->titleWidget->setShowButton(skin_button | min_button | max_button | close_button);
    connect(ui->titleWidget, SIGNAL(skp_sig_skin()),
            this, SLOT(skp_on_skin()));
    connect(ui->titleWidget, SIGNAL(skp_sig_min()),
            this, SLOT(skp_on_min()));
    connect(ui->titleWidget, SIGNAL(skp_sig_max()),
            this, SLOT(skp_on_max()));
    connect(ui->titleWidget, SIGNAL(skp_sig_close()),
            this, SLOT(skp_on_close()));

    connect(ui->tabBar, SIGNAL(currentChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));

    m_fileWidget = new SkpFileWidget(this);
    addTabWidget(m_fileWidget, QPixmap(":/ble_live.png"), tr("File"));

    //m_socketTestWidget = new SkpSocketTestWidget(this);
    //addTabWidget(m_socketTestWidget, QPixmap(":/ble_live.png"), tr("Test"));
    //m_socketTestWidget_2 = new SkpSocketTestWidget(this);
    //addTabWidget(m_socketTestWidget_2, QPixmap(":/ble_live.png"), tr("test_2"));
    //setWindowIcon(QIcon(":/logo.png"));

    m_systemTrayIcon = new QSystemTrayIcon(QIcon(":/logo.png"), this);
    connect(m_systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(skp_on_activated(QSystemTrayIcon::ActivationReason)));


    QString versionStr = QString("  version: %1").arg("skp test");
    m_statusLabel = new QLabel(versionStr, this);
    m_statusLabel->setFrameStyle(QFrame::NoFrame);
    ui->statusBar->addWidget(m_statusLabel);
    ui->statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
}

SkpMainWindow::~SkpMainWindow()
{
    delete ui;
}

void SkpMainWindow::skp_show()
{
    m_systemTrayIcon->show();
    show();
}

void SkpMainWindow::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
    QMainWindow::mouseDoubleClickEvent(mouseEvent);
    skp_change_doubleClick();
}

void SkpMainWindow::paintEvent(QPaintEvent *paintEvent)
{
    m_titleHeight = ui->titleWidget->height() + ui->tabBar->height() + 10;
    m_linearHeight = 0;
    m_statusHeight = ui->statusBar->height();
    //QMainWindow::paintEvent(paintEvent);
    //QPainter p(this);
    //QMainWindow::paintEvent(paintEvent);
    skp_draw_themed();

#ifdef Q_OS_LINUX
    ///处理在linux下状态栏不能透明
    QColor cachedColor;
    QString themeType = MOption::instance()->option("WindowBGPixmapType", "theme").toString();

    if(themeType == "bitmap") {
        cachedColor = MOption::instance()->option(OPTION_AVERAGE_COLOR, OPTION_GROUP_Theme).value<QColor>();
    } else {
        cachedColor = MOption::instance()->option("WindowBGColor", "theme").value<QColor>();
    }
    int aeroTransparent = MOption::instance()->option("AeroTransparent", "theme").toInt();
    cachedColor.setAlpha(aeroTransparent);

    QPalette palette;
    palette.setColor(QPalette::Background, cachedColor);
    //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));
    ui->statusBar->setPalette(palette);
#endif
}

void SkpMainWindow::showEvent(QShowEvent *showEvent)
{
    if(isVisible()) repaint();
    //repaint();
    QMainWindow::showEvent(showEvent);
}

void SkpMainWindow::resizeEvent(QResizeEvent *resizeEvent)
{
    if(isVisible()) repaint();
    //repaint();
    QMainWindow::resizeEvent(resizeEvent);
}

void SkpMainWindow::changeEvent(QEvent *event)
{
    //if(isVisible()) repaint();
    //repaint();
    QMainWindow::changeEvent(event);
}

void SkpMainWindow::skp_on_skin()
{
    SkpSkinThemedWidget *skinThemed = new SkpSkinThemedWidget();
    skinThemed->show();
}

void SkpMainWindow::skp_on_min()
{
    skp_change_min(true);
}

void SkpMainWindow::skp_on_max()
{
    skp_change_max();
}

void SkpMainWindow::skp_on_close()
{
    skp_change_close(SKP::Type_Quit);
}

void SkpMainWindow::skp_on_doubleClick()
{
    skp_change_doubleClick();
}

void SkpMainWindow::addTabWidget(QWidget *widget, const QPixmap &pixmap, const QString &text)
{
    ui->stackedWidget->addWidget(widget);
    ui->tabBar->addTab(pixmap, text);
}

void SkpMainWindow::skp_on_Show()
{
    skp_change_show();
}

void SkpMainWindow::skp_on_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Context:
    {
        QMenu menu;
        menu.addAction(tr("show"), this, SLOT(skp_on_Show()));
        menu.addAction(tr("hide"), this, SLOT(skp_on_min()));
        menu.addAction(tr("quit"), this, SLOT(skp_on_close()));

        QPoint globalPoint = QCursor::pos();
        menu.exec(globalPoint);
    }
        break;
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::Trigger:
    default:
        skp_on_Show();
        break;
    }
}
