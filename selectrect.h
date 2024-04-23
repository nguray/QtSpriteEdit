#ifndef SELECTRECT_H
#define SELECTRECT_H

#include <QPainter>
#include <QPoint>
#include <QRect>

class Handle : public QRect{
public:
    Handle();
    Handle(int *x,int *y);
    int     *m_x;
    int     *m_y;
};

typedef enum{
    TOP_LEFT=0,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CORNER_END
} corner_t;


class SelectRect: public QRect {
public:

    SelectRect();

    int     m_mode;

    int     m_left;
    int     m_top;
    int     m_right;
    int     m_bottom;

    int     m_idHandle;

    Handle  m_Corners[4];

    void    draw(QPainter *p);
    void    updateRect(int cellSize);
    void    reset();
    void    backupRect();
    void    rectoreRect();
    void    offSet(int dx, int dy);
    bool    hitHandle(QPoint pt);
    void    setSelectedHandle(int x,int y);
    void    normalize();
    int     widthPix();
    int     heightPix();


private:
    int     m_leftBak;
    int     m_topBak;
    int     m_rightBak;
    int     m_bottomBak;

};

#endif // SELECTRECT_H
