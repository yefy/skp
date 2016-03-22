#ifndef SKP_CHANGE_WIDGET_H
#define SKP_CHANGE_WIDGET_H

#include <QRect>

class QWidget;
class NcFramelessHelper;

namespace SKP {
enum TypeClose{
    Type_Close = 0,
    Type_Hide,
    Type_Quit
};
}



class SkpChangeWidget
{

public:
    explicit SkpChangeWidget(QWidget *parent);

public:
    void skp_change_min(bool isHide = false);
    void skp_change_max();
    void skp_change_close(SKP::TypeClose type = SKP::Type_Close);
    void skp_change_doubleClick();
    void skp_change_show();

    void skp_set_move(NcFramelessHelper *move);


private:
    QWidget *m_ChangeWidget;
    bool m_isChangeMax;
    bool m_isChangeFull;

    NcFramelessHelper *m_move;
};

#endif // SKP_CHANGE_WIDGET_H
