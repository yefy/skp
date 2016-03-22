#ifndef SKP_CUSTOM_DIALOG_H
#define SKP_CUSTOM_DIALOG_H

#include <QDialog>
#include "skp_change_widget.h"
#include "skp_themed_base.h"
#include "skp_title_widget.h"

class SkpTitleWidget;
class QVBoxLayout;
class NcFramelessHelper;

namespace Ui {
class SkpCustomDialog;
}

class SkpCustomDialog : public QDialog, public SkpChangeWidget, public SkpThemedBase
{
    Q_OBJECT

public:
    explicit SkpCustomDialog(QWidget *parent = 0);
    ~SkpCustomDialog();
    void addWidget(QWidget *widget);
    void setMovable(bool moveble);
    void setResizable(bool resizable);
    void setTitle(const QString &title);
    void setShowButton(int flags);
    void setDoubleclick(bool isDoubleClick);
    virtual void skp_close();

protected:
    void mouseDoubleClickEvent(QMouseEvent *mouseEvent);
    void paintEvent(QPaintEvent *paintEvent);
    void showEvent(QShowEvent *showEvent);
    void resizeEvent(QResizeEvent *resizeEvent);

private slots:
    void skp_on_min();
    void skp_on_max();
    void skp_on_close();
    void skp_on_doubleClick();

private:
    Ui::SkpCustomDialog *ui;
    bool m_isDoubleClick;

    SkpTitleWidget *m_titleWidget;
    QVBoxLayout *m_vBoxlayout;
    NcFramelessHelper *m_framelessHelper;
};

#endif // SKP_CUSTOM_DIALOG_H
