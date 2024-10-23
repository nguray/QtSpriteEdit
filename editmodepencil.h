#ifndef EDITMODEPENCIL_H
#define EDITMODEPENCIL_H

#include "editmode.h"

class EditModePencil : public EditMode
{
public:
    EditModePencil();

    int m_start_x;
    int m_start_y;

    void paintEvent(QWidget *w,QPainter &painter) override;
    bool mousePressEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseReleaseEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseMoveEvent(QWidget *w,QMouseEvent *event) override;
    void resizeEvent(QWidget *w,QResizeEvent *event) override;
    void initMode() override;

};

#endif // EDITMODEPENCIL_H
