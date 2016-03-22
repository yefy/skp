#ifndef SKP_CHAT_WIDGET_H
#define SKP_CHAT_WIDGET_H

#include "skp_custom_dialog.h"
#include "skp_chat_main_widget.h"

class SkpChatWidget : public SkpCustomDialog
{
public:
    SkpChatWidget(QWidget *parent = 0);
    SkpChatMainWidget *m_chatMainWidget;
};

#endif // SKP_CHAT_WIDGET_H
