#ifndef SKP_THEMED_BASE_H
#define SKP_THEMED_BASE_H

#include <QPixmap>
#include <QColor>

class QPainter;
class QWidget;

class SkpThemedBase
{
public:
    SkpThemedBase(QWidget *parent);
    virtual ~SkpThemedBase();
    void skp_register_themed();
    void skp_remove_themed();
    void skp_apply_themed();
    void skp_draw_themed();
    void skp_draw_themed(QPainter &p);
    void skp_set_Paint_Height(int title, int linear, int status);

protected:
    int m_titleHeight;
    int m_linearHeight;
    int m_statusHeight;

private:
    QWidget *m_parent;
    QPixmap m_cachedPixmap;
    QColor m_cachedColor;
    int m_aeroTransparent;
    int m_widgetTransparent;
};

extern "C" {
    void skp_updata_themed();
    void skp_update_Style_Sheet();
}

#endif // SKP_THEMED_BASE_H
