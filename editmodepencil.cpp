#include "editmodepencil.h"
#include "editarea.h"
#include <QApplication>
#include <QMouseEvent>

EditModePencil::EditModePencil()
{
    m_start_x = 0;
    m_start_y = 0;
}

void EditModePencil::initMode(){

}

void EditModePencil::paintEvent(QWidget *w,QPainter &painter)
{
    // if (m_image){
    //      painter.drawImage(QPoint(200,200),*m_image);
    // }
}

bool EditModePencil::mousePressEvent(QWidget *w,QMouseEvent *event)
{
    //-----------------------------------
    if (event->button()==Qt::LeftButton) {
        int pixelX,pixelY;
        QTransform trans = m_transform_scale * m_transform_translate;
        QPoint pt = trans.map(event->pos());
        if (m_image){
            emit ((editarea *) w)->saveImageState();
            saveStartState();
            if (QApplication::keyboardModifiers() & Qt::ControlModifier){
                if (mouseToPixel( pt.x(), pt.y(), pixelX, pixelY)){
                    restoreStartState();
                    QPainter p(m_image.get());
                    p.setRenderHint(QPainter::Antialiasing, false);
                    p.setPen(m_foreGroundColor);
                    p.drawLine( m_start_x, m_start_y, pixelX, pixelY);
                    p.end();
                    emit ((editarea *) w)->editSpriteChanged();
                    w->update();
                }
            }else{
                if (mouseToPixel( pt.x(), pt.y(), pixelX, pixelY)){
                    m_start_x = pixelX;
                    m_start_y = pixelY;
                    m_image->setPixelColor( pixelX, pixelY, m_foreGroundColor);
                    emit ((editarea *) w)->editSpriteChanged();
                    w->update();
                    return true;
                }
            }
        }
    }
    return false;
}

bool EditModePencil::mouseReleaseEvent(QWidget *w,QMouseEvent *event)
{
    //-----------------------------------
    int pixelX,pixelY;
    QTransform trans = m_transform_scale * m_transform_translate;
    QPoint pt = trans.map(event->pos());
    if (mouseToPixel( pt.x(), pt.y(), pixelX, pixelY)){
        m_start_x = pixelX;
        m_start_y = pixelY;
    }
    return false;
}

bool EditModePencil::mouseDoubleClickEvent(QWidget *w,QMouseEvent *event)
{
    return false;
}

bool EditModePencil::mouseMoveEvent(QWidget *w,QMouseEvent *event)
{
    //-----------------------------------
    if (event->buttons() & Qt::LeftButton) {
        int pixelX,pixelY;
        QTransform trans = m_transform_scale * m_transform_translate;
        QPoint pt = trans.map(event->pos());
        if (m_image){
            if (QApplication::keyboardModifiers() & Qt::ControlModifier){
                if (mouseToPixel( pt.x(), pt.y(), pixelX, pixelY)){
                    if ((pixelX!=m_start_x)||(pixelY!=m_start_y)){
                        restoreStartState();
                        QPainter p(m_image.get());
                        p.setRenderHint(QPainter::Antialiasing, false);
                        p.setPen(m_foreGroundColor);
                        p.drawLine( m_start_x, m_start_y, pixelX, pixelY);
                        p.end();
                        emit ((editarea *) w)->editSpriteChanged();
                        w->update();
                    }
                }
            }else{
                if (mouseToPixel( pt.x(), pt.y(), pixelX, pixelY)){
                    if ((pixelX!=m_start_x)||(pixelY!=m_start_y)){
                        m_start_x = pixelX;
                        m_start_y = pixelY;
                        m_image->setPixelColor( pixelX, pixelY, m_foreGroundColor);
                        emit ((editarea *) w)->editSpriteChanged();
                        w->update();
                        return true;
                    }
                }
            }
        }
    }
    return false;

}

void EditModePencil::resizeEvent(QWidget *w,QResizeEvent *event)
{

}
