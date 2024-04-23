#ifndef EDITMODEFILL_H
#define EDITMODEFILL_H

#include "editmode.h"

class EditModeFill : public EditMode
{
public:
    EditModeFill();


    void FloodFill( int fillX, int fillY, QColor fillColor);

    void paintEvent(QWidget *w,QPainter &painter) override;
    bool mousePressEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseReleaseEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseMoveEvent(QWidget *w,QMouseEvent *event) override;
    void resizeEvent(QWidget *w,QResizeEvent *event) override;

};

#endif // EDITMODEFILL_H
