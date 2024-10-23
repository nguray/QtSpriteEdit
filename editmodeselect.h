#ifndef EDITMODESELECT_H
#define EDITMODESELECT_H

#include "editmode.h"
#include "selectrect.h"

class EditModeSelect : public EditMode
{
public:
    EditModeSelect();

    QPoint  m_startPt;
    QPoint  m_endPt;

    int     m_startPixX;
    int     m_startPixY;

    SelectRect  m_selectBox;

    void    cutSelectBox();
    void    copySelectBox();
    void    pasteSelectBox();

    void paintEvent(QWidget *w,QPainter &painter) override;
    bool mousePressEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseReleaseEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QWidget *w,QMouseEvent *event) override;
    bool mouseMoveEvent(QWidget *w,QMouseEvent *event) override;
    void resizeEvent(QWidget *w,QResizeEvent *event) override;
    void initMode() override;

};

#endif // EDITMODESELECT_H
