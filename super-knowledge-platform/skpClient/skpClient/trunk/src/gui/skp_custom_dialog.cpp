#include "skp_custom_dialog.h"
#include "ui_skp_custom_dialog.h"
#include "skp_title_widget.h"
#include <QVBoxLayout>
#include "NcFramelessHelper.h"

#ifdef Q_OS_WIN
#include <qwinfunctions.h>
#endif

SkpCustomDialog::SkpCustomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SkpCustomDialog),
    SkpChangeWidget(this),
    SkpThemedBase(this),
    m_titleWidget(new SkpTitleWidget(this)),
    m_vBoxlayout(new QVBoxLayout(this)),
    m_framelessHelper(new NcFramelessHelper),
    m_isDoubleClick(true)
{
    ui->setupUi(this);
    m_titleWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    connect(m_titleWidget, SIGNAL(skp_sig_min()),
            this, SLOT(skp_on_min()));
    connect(m_titleWidget, SIGNAL(skp_sig_max()),
            this, SLOT(skp_on_max()));
    connect(m_titleWidget, SIGNAL(skp_sig_close()),
            this, SLOT(skp_on_close()));

    m_vBoxlayout->setSpacing(0);
    //m_vBoxlayout->setContentsMargins(0, 0, 0, 0);
    m_vBoxlayout->setContentsMargins(5, 5, 5, 5);
    m_vBoxlayout->addWidget(m_titleWidget);
    setLayout(m_vBoxlayout);

    m_framelessHelper->setWidgetMovable(true);
    m_framelessHelper->setWidgetResizable(true);
    m_framelessHelper->activateOn(this);

    skp_set_move(m_framelessHelper);

#ifdef Q_OS_WIN
    //QtWin::enableBlurBehindWindow(this);
#endif

}

SkpCustomDialog::~SkpCustomDialog()
{
    delete ui;
}

void SkpCustomDialog::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
    QDialog::mouseDoubleClickEvent(mouseEvent);
    if(m_isDoubleClick)
        skp_change_doubleClick();
}

void SkpCustomDialog::paintEvent(QPaintEvent *paintEvent)
{
    QDialog::paintEvent(paintEvent);
    skp_draw_themed();
}

void SkpCustomDialog::showEvent(QShowEvent *showEvent)
{
    if(isVisible()) repaint();
    QDialog::showEvent(showEvent);
}

void SkpCustomDialog::resizeEvent(QResizeEvent *resizeEvent)
{
    QDialog::resizeEvent(resizeEvent);
}

void SkpCustomDialog::skp_on_min()
{
    skp_change_min();
}

void SkpCustomDialog::skp_on_max()
{
    skp_change_max();
}

void SkpCustomDialog::skp_on_close()
{
    skp_close();
}

void SkpCustomDialog::skp_close()
{
    skp_change_close();
}

void SkpCustomDialog::skp_on_doubleClick()
{
    skp_change_doubleClick();
}

void SkpCustomDialog::addWidget(QWidget *widget)
{
    widget->setParent(this);

    m_vBoxlayout->addWidget(widget);
}

void SkpCustomDialog::setMovable(bool moveble)
{
    m_framelessHelper->setWidgetMovable(moveble);
}

void SkpCustomDialog::setResizable(bool resizable)
{
    m_framelessHelper->setWidgetResizable(resizable);
}

void SkpCustomDialog::setTitle(const QString &title)
{
    m_titleWidget->setTitle(title);
}

void SkpCustomDialog::setShowButton(int flags)
{
    m_titleWidget->setShowButton(flags);
}

void SkpCustomDialog::setDoubleclick(bool isDoubleClick)
{
    m_isDoubleClick = isDoubleClick;
}
