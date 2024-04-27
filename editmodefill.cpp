#include "editmodefill.h"
#include "editarea.h"
#include <stack>
#include <QMouseEvent>

EditModeFill::EditModeFill()
{

}

void EditModeFill::FloodFill( int fillX, int fillY, QColor fillColor)
{
    int                 width, height;
    QColor              targetColor,curColor;
    QPoint              pt;
    std::stack<QPoint>  stk;
    int                 x,y;
    int                 startX,startY;
    int                 xEndLine,xStartLine;
    bool                fStartNord,
                        fNord,
                        fStartSud,
                        fSud;
    //-----------------------------------------------------------------------

    if (!m_image) return;

    targetColor = m_image->pixelColor( fillX, fillY);

    if ((!targetColor.isValid())||(targetColor==fillColor)) return;

    width = m_image->width();
    height = m_image->height();

    stk.push(QPoint(fillX,fillY));

    while (!stk.empty()){

        startX = stk.top().x();
        startY = stk.top().y();
        stk.pop();

        //-- Vérifier au Nord
        fStartNord = false;

        if (startY>0){
            curColor = m_image->pixelColor(startX,startY-1);
            if (curColor.isValid()){
                if (curColor==targetColor){
                    stk.push(QPoint(startX,startY-1));
                    fStartNord = true;
                }
            }
        }

        //-- Vérifier au sud
        fStartSud = false;
        if ((startY+1)<height){
            curColor = m_image->pixelColor(startX,startY+1);
            if (curColor.isValid()){
                if (curColor==targetColor){
                    stk.push(QPoint(startX,startY+1));
                    fStartSud = true;
                }
            }
        }

        y = startY;
        //-- Aller vers l'est
        xEndLine = startX;
        fNord = fStartNord;
        fSud = fStartSud;
        if (xEndLine<(width-1)){

            x = xEndLine + 1;
            while(x<width){
                curColor = m_image->pixelColor(x,y);
                if (!curColor.isValid()) break;
                if (curColor!=targetColor){
                    break;
                }

                //-- Vérifier au Nord
                if ((y-1)>0){
                    curColor = m_image->pixelColor(x,y-1);
                    if ((curColor.isValid())&&(curColor==targetColor)){
                        if (!fNord){
                            stk.push(QPoint(x,y-1));
                            fNord = true;
                        }
                    }else{
                        fNord = false;
                    }
                }else{
                    fNord = false;
                }

                //-- Vérifier au sud
                if (y<(height-1)){
                    curColor = m_image->pixelColor(x,y+1);
                    if (curColor.isValid()&&(curColor==targetColor)){
                        if (!fSud){
                            stk.push(QPoint(x,y+1));
                            fSud = true;
                        }
                    }else{
                        fSud = false;
                    }
                }else{
                    fSud = false;
                }
                xEndLine = x;
                x++;
            }

        }else{
            xEndLine = width-1;
        }

        //-- Aller vers l'ouest
        xStartLine = startX;
        fNord = fStartNord;
        fSud = fStartSud;
        if (xStartLine>0){
            x = xStartLine - 1;
            while(x>=0){

                curColor = m_image->pixelColor(x,y);
                if (!curColor.isValid()) break;
                if (curColor!=targetColor) break;

                //-- Vérifier au Nord
                if ((y-1)>0){
                    curColor = m_image->pixelColor(x,y-1);
                    if (curColor.isValid()&&(curColor==targetColor)){
                        if (!fNord){
                            stk.push( QPoint(x,y-1));
                            fNord = true;
                        }
                    }else{
                        fNord = false;
                    }
                }else{
                    fNord = false;
                }

                //-- Vérifier au sud
                if (y<(height-1)){
                    curColor = m_image->pixelColor(x,y+1);
                    if (curColor.isValid()&&(curColor==targetColor)){
                        if (!fSud){
                            stk.push(QPoint(x,y+1));
                            fSud = true;
                        }
                    }else{
                        fSud = false;
                    }
                }else{
                    fSud = false;
                }
                xStartLine = x;
                x--;
            }

        }else{
            xStartLine = 0;
        }
        //-- Tracer la line
        for(x=xStartLine;x<=xEndLine;x++){
            m_image->setPixelColor( x, y, fillColor);

        }

    }


}

void EditModeFill::paintEvent(QWidget *w,QPainter &painter)
{
    // if (m_image){
    //      painter.drawImage(QPoint(200,200),*m_imageBackup);
    // }
}

bool EditModeFill::mousePressEvent(QWidget *w,QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton) {
        int pixelX,pixelY;
        QPoint pt = event->pos();
        if (m_image){
            if (mouseToPixel( pt.x(), pt.y(), pixelX, pixelY)){
                backup();
                FloodFill( pixelX, pixelY, m_foreGroundColor);
                emit ((editarea *) w)->editSpriteChanged();
                w->update();
            }
        }
    }
    return false;
}

bool EditModeFill::mouseReleaseEvent(QWidget *w,QMouseEvent *event)
{
    return false;
}

bool EditModeFill::mouseDoubleClickEvent(QWidget *w,QMouseEvent *event)
{
    return false;
}

bool EditModeFill::mouseMoveEvent(QWidget *w,QMouseEvent *event)
{

    return false;
}

void EditModeFill::resizeEvent(QWidget *w,QResizeEvent *event)
{

}
