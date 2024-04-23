#include "editarea.h"
#include "editmode.h"
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>

editarea::editarea(QWidget *parent)
    : QWidget{parent}
{
    m_editMode->m_foreGroundColor = QColor(255,0,0);
    m_editMode->m_backGroundColor = QColor(0,0,0,0);
    m_editMode->m_pixWidth = 32;
    m_editMode->m_pixHeight = 32;
    m_editMode->m_cellSize = 24;
    m_editMode->m_image = NULL;
    m_editMode->m_imageBackup = NULL;
    m_editMode->m_imageCopy = NULL;
    //m_image = new QImage(m_pixWidth,m_pixHeight,QImage::Format_ARGB32);
    //m_image->fill(QColor(0,0,0,0));

    m_editMode = &m_editModePencil;

}

editarea::~editarea()
{

}

void editarea::setEditSprite(QImage *sprite)
{
    if (sprite){
        m_editMode->m_pixWidth = sprite->width();
        m_editMode->m_pixHeight = sprite->height();
        m_editMode->m_image = sprite;
        if (m_editMode->m_imageBackup){
            delete m_editMode->m_imageBackup;
        }
        m_editMode->m_imageBackup = new QImage(sprite->width(),sprite->height(),sprite->format());
        m_editMode->m_imageBackup->fill(QColor(0,0,0,0));
        update();
    }
}

void editarea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    auto w = width();
    auto h = height();

    if (w>h){
        if (m_editMode->m_pixHeight>m_editMode->m_pixWidth){
            m_editMode->m_cellSize = h/m_editMode->m_pixHeight;
        }else{
            m_editMode->m_cellSize = h/m_editMode->m_pixWidth;
        }
    }else{
        if (m_editMode->m_pixHeight>m_editMode->m_pixWidth){
            m_editMode->m_cellSize = w/m_editMode->m_pixHeight;
        }else{
            m_editMode->m_cellSize = w/m_editMode->m_pixWidth;
        }
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setClipRegion(QRegion(0,
                                  0,
                                  m_editMode->m_pixWidth*m_editMode->m_cellSize+2, m_editMode->m_pixHeight*m_editMode->m_cellSize+2,
                                  QRegion::Rectangle));

    int x,y;
    painter.setPen(QPen(QBrush(QColor(50,50,50)),0));
    for (int i=0;i<=m_editMode->m_pixHeight;i++){
        y = i*m_editMode->m_cellSize;
        for (int j=0;j<=m_editMode->m_pixWidth;j++){
            x = j*m_editMode->m_cellSize;
            painter.drawPoint(x,y);
        }
    }


    //--
    drawEditImage(&painter);

    //--
    m_editMode->paintEvent(this,painter);

    //m_selectBox.draw(&painter);

    //painter.fillRect(10, 10, 50, 50, QBrush(QColor(100,0,0,10)));
    //painter.drawImage(QPoint(0,0),*m_image);

}

void editarea::drawEditImage(QPainter *p)
{
    int     x,y;
    //---------------------------------------------------------
    if (m_editMode->m_image){
        int d = m_editMode->m_cellSize-2;
        for (int i=0;i<m_editMode->m_pixHeight;i++){
            y = i*m_editMode->m_cellSize;
            for (int j=0;j<m_editMode->m_pixWidth;j++){
                QColor c = m_editMode->m_image->pixelColor(j, i);
                x = j*m_editMode->m_cellSize;
                p->fillRect(x+1, y+1, d, d,QBrush(c));
            }
        }
    }
}

void editarea::setForeGroundColor(QColor newColor)
{
    m_editMode->m_foreGroundColor = newColor;
}

void editarea::setBackGroundColor(QColor newColor)
{
    m_editMode->m_backGroundColor = newColor;
}

void editarea::setSelectMode()
{
    m_editModeSelect.m_selectBox.reset();
    m_editMode = &m_editModeSelect;
    update();
}

void editarea::setPencilMode()
{
    m_editMode = &m_editModePencil;
    update();

}

void editarea::setRectangleMode()
{
    m_editModeRectangle.m_selectBox.reset();
    m_editMode = &m_editModeRectangle;
    update();

}

void editarea::setEllipseMode()
{
    m_editModeEllipse.m_selectBox.reset();
    m_editMode = &m_editModeEllipse;
    update();

}

void editarea::setFloodFillMode()
{
    m_editMode = &m_editModeFill;
    update();

}

void editarea::cutSelectBox()
{
    //-----------------------------------
    if (m_editMode==&m_editModeSelect){
        if (m_editModeSelect.m_selectBox.width()&&m_editModeSelect.m_selectBox.height()){
            m_editModeSelect.cutSelectBox();
            emit editSpriteChanged();
            update();
        }
    }

}

void editarea::copySelectBox()
{
    //-----------------------------------
    if (m_editMode==&m_editModeSelect){
        if (m_editModeSelect.m_selectBox.width()&&m_editModeSelect.m_selectBox.height()){
            m_editModeSelect.copySelectBox();
            update();
        }
    }
}

void editarea::pasteSelectBox()
{
    //-----------------------------------
    if (m_editMode!=&m_editModeSelect){
        setSelectMode();
    }
    m_editModeSelect.pasteSelectBox();
    m_editModeSelect.m_selectBox.updateRect(m_editMode->m_cellSize);
    emit editSpriteChanged();
    update();
}

void editarea::mousePressEvent(QMouseEvent *event)
{
    m_editMode->mousePressEvent(this, event);

}

void editarea::mouseReleaseEvent(QMouseEvent *event)
{
    m_editMode->mouseReleaseEvent(this, event);

}

void editarea::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_editMode->mouseDoubleClickEvent(this, event);

}

void editarea::mouseMoveEvent(QMouseEvent *event)
{
    m_editMode->mouseMoveEvent(this, event);

}

void editarea::resizeEvent(QResizeEvent *event)
{
    m_editMode->resizeEvent(this, event);

}
