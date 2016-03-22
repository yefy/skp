#ifndef SKP_TITLE_WIDGET_H
#define SKP_TITLE_WIDGET_H

#include <QWidget>

namespace Ui {
class SkpTitleWidget;
}

enum{
    min_button    = 0x00000001,
    skin_button   = 0x00000002,
    menu_button   = 0x00000004,
    close_button  = 0x00000008,
    help_button   = 0x00000010,
    max_button    = 0x00000020,
    USER_HINT1  = 0x00000080,
    USER_HINT2  = 0x00000100,
    USER_HINT3  = 0x00000200
};

class SkpTitleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SkpTitleWidget(QWidget *parent = 0);
    ~SkpTitleWidget();
    void setTitle(const QString &title);
    void setShowButton(int flags);

protected:
    void mouseDoubleClickEvent(QMouseEvent *mouseEvent);
    void resizeEvent(QResizeEvent *resizeEvent);
    void showEvent(QShowEvent *showEvent);

private:
    Ui::SkpTitleWidget *ui;

signals:
    void skp_sig_skin();
    void skp_sig_min();
    void skp_sig_max();
    void skp_sig_close();
    void skp_sig_doubleClick();

private slots:
    void skp_on_skin();
    void skp_on_min();
    void skp_on_max();
    void skp_on_close();
};

#endif // SKP_TITLE_WIDGET_H
