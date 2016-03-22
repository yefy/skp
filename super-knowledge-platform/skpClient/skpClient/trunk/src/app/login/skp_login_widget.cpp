#include "skp_login_widget.h"
#include "skp_login_main_widget.h"

SkpLoginWidget::SkpLoginWidget(QWidget *parent) :
    SkpCustomDialog(parent)
{
    addWidget(new SkpLoginMainWidget(this));
    setTitle(tr("login"));
    setShowButton(min_button|close_button);
    setWindowTitle(tr("login"));
    setFixedSize(430, 330);
    setResizable(false);
    setDoubleclick(false);
    skp_set_Paint_Height(76, 0, 0);
}
