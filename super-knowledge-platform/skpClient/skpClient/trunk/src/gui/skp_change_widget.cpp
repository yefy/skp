#include "skp_change_widget.h"
#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include "NcFramelessHelper.h"

SkpChangeWidget::SkpChangeWidget(QWidget *parent) :
    m_ChangeWidget(parent),
    m_isChangeMax(false),
    m_isChangeFull(false),
    m_move(NULL)
{
}

void SkpChangeWidget::skp_set_move(NcFramelessHelper *move)
{
    m_move = move;
}


void SkpChangeWidget::skp_change_min(bool isHide)
{
    if(m_isChangeFull)
        return;

    if(isHide) {
        m_ChangeWidget->hide();
    } else {
        m_ChangeWidget->showMinimized();
    }
}

void SkpChangeWidget::skp_change_max()
{
    if(m_isChangeFull)
        return;

    if(m_isChangeMax) {
        m_move->setWidgetResizable(true);
        m_move->setWidgetMovable(true);
        m_ChangeWidget->showNormal();
    } else {
        m_move->setWidgetResizable(false);
        m_move->setWidgetMovable(false);
        m_ChangeWidget->showMaximized();
    }

    m_isChangeMax = !m_isChangeMax;
}

void SkpChangeWidget::skp_change_close(SKP::TypeClose type)
{
    switch (type) {
    case SKP::Type_Close:
        m_ChangeWidget->close();
        break;
    case SKP::Type_Hide:
        m_ChangeWidget->hide();
        break;
    case SKP::Type_Quit:
    default:
        qApp->quit();
        break;
    }
}

void SkpChangeWidget::skp_change_doubleClick()
{
    if(m_isChangeFull) {
        m_move->setWidgetResizable(true);
        m_move->setWidgetMovable(true);
        m_ChangeWidget->showNormal();
        if(m_isChangeMax) {
            m_move->setWidgetResizable(false);
            m_move->setWidgetMovable(false);
            m_ChangeWidget->showMaximized();
        } /*else {
            m_move->setWidgetResizable(true);
            m_move->setWidgetMovable(true);
            m_ChangeWidget->showNormal();
        }*/
    } else {
        m_move->setWidgetResizable(false);
        m_move->setWidgetMovable(false);
        m_ChangeWidget->showFullScreen();
    }

    m_isChangeFull = !m_isChangeFull;

}

void SkpChangeWidget::skp_change_show()
{
    if(m_isChangeFull) {
        m_move->setWidgetResizable(false);
        m_move->setWidgetMovable(false);
        m_ChangeWidget->showFullScreen();
    } else if(m_isChangeMax) {
        m_move->setWidgetResizable(false);
        m_move->setWidgetMovable(false);
        m_ChangeWidget->showMaximized();
    } else {
        m_move->setWidgetResizable(true);
        m_move->setWidgetMovable(true);
        m_ChangeWidget->show();
    }
}

