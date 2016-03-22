#include "skp_title_widget.h"
#include "ui_skp_title_widget.h"
#include <QTimer>
#include <QDebug>

SkpTitleWidget::SkpTitleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkpTitleWidget)
{
    ui->setupUi(this);

    ///子类事件给父类   子类不响应事件
    //setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->skinToolButton->setPixmap(QPixmap(":/skin_button.png"));
    ui->minToolButton->setPixmap(QPixmap(":/sys_button_min.png"));
    ui->maxToolButton->setPixmap(QPixmap(":/sys_button_restore.png"));
    ui->closeToolButton->setPixmap(QPixmap(":/sys_button_close.png"));



    connect(ui->skinToolButton, SIGNAL(clicked()),
            this, SLOT(skp_on_skin()));
    connect(ui->minToolButton, SIGNAL(clicked()),
            this, SLOT(skp_on_min()));
    connect(ui->maxToolButton, SIGNAL(clicked()),
            this, SLOT(skp_on_max()));
    connect(ui->closeToolButton, SIGNAL(clicked()),
            this, SLOT(skp_on_close()));

    ui->skinToolButton->setVisible(false);
    ui->minToolButton->setVisible(false);
    ui->maxToolButton->setVisible(false);
    ui->closeToolButton->setVisible(false);
}

SkpTitleWidget::~SkpTitleWidget()
{
    delete ui;
}

void SkpTitleWidget::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
    QWidget::mouseDoubleClickEvent(mouseEvent);
    emit skp_sig_doubleClick();
}

void SkpTitleWidget::resizeEvent(QResizeEvent *resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
}

void SkpTitleWidget::showEvent(QShowEvent *showEvent)
{
    QWidget::showEvent(showEvent);
}

void SkpTitleWidget::skp_on_skin()
{
    emit skp_sig_skin();
}

void SkpTitleWidget::skp_on_min()
{
    emit skp_sig_min();
}

void SkpTitleWidget::skp_on_max()
{
    emit skp_sig_max();
}

void SkpTitleWidget::skp_on_close()
{
    emit skp_sig_close();
}

void SkpTitleWidget::setTitle(const QString &title)
{
    ui->tileLabel->setText(title);
}

void SkpTitleWidget::setShowButton(int flags)
{
    if(flags & skin_button)
    {
        ui->skinToolButton->setVisible(true);
    }

    if(flags & min_button)
    {
        ui->minToolButton->setVisible(true);
    }

    if(flags & max_button)
    {
        ui->maxToolButton->setVisible(true);
    }

    if(flags & close_button)
    {
        ui->closeToolButton->setVisible(true);
    }
}
