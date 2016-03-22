#include "skp_skin_themed_widget.h"
#include "MSkinChanger.h"

SkpSkinThemedWidget::SkpSkinThemedWidget(QWidget *parent) :
    SkpCustomDialog(parent)
{
    addWidget(new MSkinChanger(this));
    setTitle(tr("change skin"));
    setShowButton(close_button);
    setWindowTitle(tr("change skin"));
    setFixedSize(490, 380);
    setResizable(false);
    setDoubleclick(false);
    skp_set_Paint_Height(76, 0, 0);
}
