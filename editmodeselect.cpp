#include "editmodeselect.h"
#include "editarea.h"
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>

EditModeSelect::EditModeSelect():EditMode{}
{

}

void EditModeSelect::cutSelectBox()
{
    QRect srcRect;
    //----------------------------------------------------
    //-- Make a Copy
    // if (m_imageCopy){
    //     delete m_imageCopy;
    // }
    m_selectBox.normalize();
    int w = m_selectBox.m_right - m_selectBox.m_left + 1;
    int h = m_selectBox.m_bottom - m_selectBox.m_top + 1;
    m_imageCopy = std::make_shared<QImage>(w , h, m_image->format());
    m_imageCopy->fill(QColor(0,0,0,0));
    QPainter painter(m_imageCopy.get());
    srcRect.setCoords(m_selectBox.m_left, m_selectBox.m_top, m_selectBox.m_right, m_selectBox.m_bottom);
    painter.drawImage(QPoint(0,0), *m_image, srcRect);
    painter.end();

    //-- Fill select area with background color
    for(int y=m_selectBox.m_top;y<=m_selectBox.m_bottom;y++){
        for(int x=m_selectBox.m_left;x<=m_selectBox.m_right;x++){
            m_image->setPixelColor(x,y,m_backGroundColor);
        }
    }

    //--
    m_selectBox.reset();

}

void EditModeSelect::copySelectBox()
{
    QRect srcRect;
    //----------------------------------------------------
    // if (m_imageCopy){
    //     delete m_imageCopy;
    // }
    m_selectBox.normalize();
    int w = m_selectBox.m_right - m_selectBox.m_left + 1;
    int h = m_selectBox.m_bottom - m_selectBox.m_top + 1;
    m_imageCopy = std::make_shared<QImage>(w , h, m_image->format());
    m_imageCopy->fill(QColor(0,0,0,0));
    QPainter painter(m_imageCopy.get());
    srcRect.setCoords(m_selectBox.m_left, m_selectBox.m_top, m_selectBox.m_right, m_selectBox.m_bottom);
    painter.drawImage(QPoint(0,0), *m_image, srcRect);
    painter.end();

    //--
    m_selectBox.reset();

}

void EditModeSelect::pasteSelectBox()
{
    //----------------------------------------------------
    if (m_imageCopy){
        saveState();
        //-- Put in the middle of the edit area
        m_selectBox.m_left = (m_image->width()-m_imageCopy->width())/2;
        m_selectBox.m_top = (m_image->height()-m_imageCopy->height())/2;
        m_selectBox.m_right = m_selectBox.m_left + m_imageCopy->width()-1;
        m_selectBox.m_bottom = m_selectBox.m_top + m_imageCopy->height()-1;
        m_selectBox.m_mode = 2;
        QPainter painter(m_image.get());
        painter.drawImage(QPoint(m_selectBox.m_left,m_selectBox.m_top), *m_imageCopy);
        painter.end();
    }

}

void EditModeSelect::paintEvent(QWidget *w,QPainter &painter)
{

    // painter.setRenderHint(QPainter::Antialiasing);
    // painter.setClipRegion(QRegion(0,
    //                               0,
    //                               m_pixWidth*m_cellSize+2, m_pixHeight*m_cellSize+2,
    //                               QRegion::Rectangle));
    //--
    m_selectBox.draw(&painter);

    // if (m_imageCopy){
    //     painter.drawImage(QPoint(240,100),*m_imageCopy);
    // }

}

bool EditModeSelect::mousePressEvent(QWidget *w,QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton) {
        int pixelX,pixelY;
        QTransform trans = m_transform_scale * m_transform_translate;
        QPoint pt = trans.map(event->pos());
        if (m_selectBox.m_mode==1){
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
                    w->update();
                    return true;
                }
            }
        }else if (m_selectBox.m_mode==2){
            if (m_selectBox.contains(pt)){
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
                w->update();
                return true;
            }
        }
    }
    return false;

}

bool EditModeSelect::mouseReleaseEvent(QWidget *w,QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){
        if (m_selectBox.m_mode==0){
            if (!m_selectBox.isNull()){
                m_selectBox.backupRect();
                m_selectBox.m_mode = 1;
                w->update();
                return true;
            }
        }
    }
    return false;
}

bool EditModeSelect::mouseDoubleClickEvent(QWidget *w,QMouseEvent *event)
{
    return false;
}

bool EditModeSelect::mouseMoveEvent(QWidget *w,QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int pixelX,pixelY;
        QTransform trans = m_transform_scale * m_transform_translate;
        QPoint pt = trans.map(event->pos());
        if (m_selectBox.m_mode==1){
            if (m_selectBox.m_idHandle!=-1){
                mouseToPixel( pt.x(), pt.y(), pixelX, pixelY);
                m_selectBox.backupRect();
                m_selectBox.setSelectedHandle(pixelX,pixelY);
                if ((m_selectBox.widthPix()<2)||(m_selectBox.heightPix()<2)){
                    m_selectBox.rectoreRect();
                }
                m_selectBox.updateRect(m_cellSize);
                w->update();
                return true;
            }else {
                mouseToPixel( pt.x(), pt.y(), pixelX, pixelY);
                int dx = (pixelX - m_startPixX);
                int dy = (pixelY - m_startPixY);
                if (dx||dy){
                    m_selectBox.rectoreRect();
                    m_selectBox.offSet(dx,dy);
                    m_selectBox.updateRect(m_cellSize);
                    w->update();
                }
                return true;
            }
        }else if (m_selectBox.m_mode==2){
            QRect   srcRect;
            mouseToPixel( pt.x(), pt.y(), pixelX, pixelY);
            restoreStartState();
            int dx = (pixelX - m_startPixX);
            int dy = (pixelY - m_startPixY);
            if (dx||dy){
                m_selectBox.rectoreRect();
                m_selectBox.offSet(dx,dy);
                m_selectBox.updateRect(m_cellSize);
                QPainter painter(m_image.get());
                srcRect.setCoords(m_selectBox.m_left, m_selectBox.m_top, m_selectBox.m_right, m_selectBox.m_bottom);
                painter.drawImage(QPoint(m_selectBox.m_left,m_selectBox.m_top), *m_imageCopy);
                painter.end();
                w->update();
                emit ((editarea *) w)->editSpriteChanged();
                return true;
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
                w->update();
                return true;
            }
        }

    }
    return false;

}

void EditModeSelect::resizeEvent(QWidget *w,QResizeEvent *event)
{

}
