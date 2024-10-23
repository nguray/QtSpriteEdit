#ifndef EDITMODERECTANGLE_H
#define EDITMODERECTANGLE_H

#include "editmode.h"
#include "selectrect.h"

class EditModeRectangle : public EditMode
{
public:
    EditModeRectangle();

    QPoint  m_startPt;
    QPoint  m_endPt;

    int     m_startPixX;
    int     m_startPixY;

    SelectRect  m_selectBox;

    void paintEvent(QWidget *w,QPainter &painter) override;
    bool mousePressEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseReleaseEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseMoveEvent(QWidget *w,QMouseEvent *event) override;
    void resizeEvent(QWidget *w,QResizeEvent *event) override;
    void initMode() override;

    void drawRectangle(int l,int t,int r,int b);


};

#endif // EDITMODERECTANGLE_H
