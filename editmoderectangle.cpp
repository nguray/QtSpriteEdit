#include "editmoderectangle.h"
#include "editarea.h"
#include <QMouseEvent>

EditModeRectangle::EditModeRectangle()
{

}

void EditModeRectangle::paintEvent(QWidget *w,QPainter &painter)
{

    //painter.setRenderHint(QPainter::Antialiasing);
    // if (m_imageBackup){
    //      painter.drawImage(QPoint(200,200),*m_imageBackup);
    // }

    // painter.setClipRegion(QRegion(0,
    //                               0,
    //                               m_pixWidth*m_cellSize+2, m_pixHeight*m_cellSize+2,
    //                               QRegion::Rectangle));

    //--
    m_selectBox.draw(&painter);

}

bool EditModeRectangle::mousePressEvent(QWidget *w,QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton) {
        int pixelX,pixelY;
        QTransform trans = m_transform_scale * m_transform_translate;
        QPoint pt = trans.map(event->pos());
        if (m_selectBox.m_mode){
            if (m_selectBox.hitHandle(pt)){
                mouseToPixel( pt.x(), pt.y(), pixelX, pixelY);
                m_startPixX = pixelX;
                m_startPixY = pixelY;
            }else if (m_selectBox.contains(pt)){
                m_selectBox.backupRect();
                mouseToPixel( pt.x(), pt.y(), pixelX, pixelY);
                m_startPixX = pixelX;
                m_startPixY = pixelY;
            }else{
                if (mouseToPixel( pt.x(), pt.
                                         y(), pixelX, pixelY)){
                    m_startPt = QPoint(pixelX,pixelY);
                    m_endPt = m_startPt;
                    m_selectBox.reset();
                    m_selectBox.m_left = m_startPt.x();
                    m_selectBox.m_top = m_startPt.y();
                    m_selectBox.m_right = m_endPt.x();
                    m_selectBox.m_bottom = m_endPt.y();
                    backup();
                    w->update();
                    return true;
                }
            }
        }else{
            if (mouseToPixel( pt.x(), pt.
                                     y(), pixelX, pixelY)){
                m_startPt = QPoint(pixelX,pixelY);
                m_endPt = m_startPt;
                m_selectBox.reset();
                m_selectBox.m_left = m_startPt.x();
                m_selectBox.m_top = m_startPt.y();
                m_selectBox.m_right = m_endPt.x();
                m_selectBox.m_bottom = m_endPt.y();
                backup();
                w->update();
                return true;
            }
        }
    }
    return false;

}

bool EditModeRectangle::mouseReleaseEvent(QWidget *w,QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton) {
        if (!m_selectBox.isNull()){
            m_selectBox.backupRect();
            m_selectBox.m_mode = 1;
            w->update();
            return true;
        }
    }
    return false;
}

bool EditModeRectangle::mouseDoubleClickEvent(QWidget *w,QMouseEvent *event)
{
    return false;
}

void EditModeRectangle::drawRectangle(int l,int t,int r,int b)
{
    QRect   rect;
    QPainter p(m_image.get());
    p.setPen(QPen(QBrush(m_foreGroundColor),0));
    rect.setCoords(l, t, r-1, b-1);
    p.drawRect(rect);
    p.end();
}


bool EditModeRectangle::mouseMoveEvent(QWidget *w,QMouseEvent *event)
{
    //--------------------------------------------------------
    if (event->buttons() & Qt::LeftButton) {
        int     pixelX,pixelY;
        bool    fDraw=false;
        QTransform trans = m_transform_scale * m_transform_translate;
        QPoint pt = trans.map(event->pos());
        if (m_selectBox.m_mode){
            if (m_selectBox.m_idHandle!=-1){
                mouseToPixel( pt.x(), pt.y(), pixelX, pixelY);
                m_selectBox.backupRect();
                m_selectBox.setSelectedHandle(pixelX,pixelY);
                if ((m_selectBox.widthPix()<2)||(m_selectBox.heightPix()<2)){
                    m_selectBox.rectoreRect();
                }
                m_selectBox.updateRect(m_cellSize);
                //-- Draw Rectangle
                fDraw = true;
            }else{
                mouseToPixel( pt.x(), pt.y(), pixelX, pixelY);
                int dx = (pixelX - m_startPixX);
                int dy = (pixelY - m_startPixY);
                if (dx||dy){
                    m_selectBox.rectoreRect();
                    m_selectBox.offSet(dx,dy);
                    m_selectBox.updateRect(m_cellSize);
                    //-- Draw Rectangle
                    fDraw = true;
                }

            }
        }else{
            if (mouseToPixel( pt.x(), pt.y(), pixelX, pixelY)){
                m_endPt = QPoint(pixelX,pixelY);
                int left = m_startPt.x();
                int top = m_startPt.y();
                int right = m_endPt.x();
                int bottom = m_endPt.y();
                if (left>right){
                    int d = left;
                    left = right;
                    right = d;
                }
                if (top>bottom){
                    int d = top;
                    top = bottom;
                    bottom = d;
                }
                m_selectBox.m_left = left;
                m_selectBox.m_top = top;
                m_selectBox.m_right = right;
                m_selectBox.m_bottom = bottom;
                m_selectBox.updateRect(m_cellSize);

                //-- Draw Rectangle
                fDraw = true;
            }
        }

        if (fDraw){
            //-- Draw Rectangle
            restore();
            drawRectangle( m_selectBox.m_left, m_selectBox.m_top, m_selectBox.m_right, m_selectBox.m_bottom);
            emit ((editarea *) w)->editSpriteChanged();
            w->update();
            return true;
        }

    }
    return false;

}

void EditModeRectangle::resizeEvent(QWidget *w,QResizeEvent *event)
{

}
