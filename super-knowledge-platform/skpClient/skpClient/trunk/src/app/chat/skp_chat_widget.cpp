#include "skp_chat_widget.h"

SkpChatWidget::SkpChatWidget(QWidget *parent) :
    SkpCustomDialog(parent)
{
    m_chatMainWidget = new SkpChatMainWidget(this);
    addWidget(m_chatMainWidget);
    setTitle(tr("chat"));
    setShowButton(min_button|close_button);
    setWindowTitle(tr("chat"));
    setFixedSize(590, 520);
    setResizable(false);
    setDoubleclick(false);
    skp_set_Paint_Height(76, 0, 0);
}
