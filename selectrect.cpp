#include "selectrect.h"

Handle::Handle():QRect{},m_x(NULL),m_y(NULL)
{
}

Handle::Handle(int *ptr_x, int *ptr_y):QRect{},m_x(ptr_x),m_y(ptr_y)
{
}

SelectRect::SelectRect():QRect{},m_mode(0),m_idHandle(-1)
{
    m_Corners[TOP_LEFT] = Handle(&m_left,&m_top);
    m_Corners[TOP_RIGHT] = Handle(&m_right,&m_top);
    m_Corners[BOTTOM_LEFT] = Handle(&m_left,&m_bottom);
    m_Corners[BOTTOM_RIGHT] = Handle(&m_right,&m_bottom);

}

void SelectRect::draw(QPainter *p)
{
    //---------------------------------------------------------
    if (!isNull()){
        p->fillRect(*this,QBrush(QColor(0,0,100,32)));
        if (m_mode==1){
            //-- TopLeft Handle
            p->fillRect(m_Corners[TOP_LEFT],QBrush(QColor(200,0,0,128)));
            //-- TopRight Handle
            p->fillRect(m_Corners[TOP_RIGHT],QBrush(QColor(200,0,0,128)));
            //-- BottomLeft Handle
            p->fillRect(m_Corners[BOTTOM_LEFT],QBrush(QColor(200,0,0,128)));
            //-- BottomRight Handle
            p->fillRect(m_Corners[BOTTOM_RIGHT],QBrush(QColor(200,0,0,128)));
        }
    }
}

void SelectRect::updateRect(int cellSize)
{
    int offSet = 8;
    //---------------------------------------------------------
    int x0 = m_left*cellSize;
    int y0 = m_top*cellSize;
    int x1 = m_right*cellSize+cellSize;
    int y1 = m_bottom*cellSize+cellSize;
    setCoords(x0, y0,x1, y1);
    m_Corners[TOP_LEFT].setRect(left()-4,top()-4,offSet,offSet);
    m_Corners[TOP_RIGHT].setRect(right()-4,top()-4,offSet,offSet);
    m_Corners[BOTTOM_LEFT].setRect(left()-4,bottom()-4,offSet,offSet);
    m_Corners[BOTTOM_RIGHT].setRect(right()-4,bottom()-4,offSet,offSet);

}

void SelectRect::reset()
{
    //---------------------------------------------------------
    m_mode = 0;
    m_left = 0;
    m_right = 0;
    m_top = 0;
    m_bottom = 0;
    setCoords(-100,-100,-100,-100);
    for(int i=0;i<CORNER_END;i++){
        m_Corners[i].setRect(0,0,0,0);
    }

}

void SelectRect::offSet(int dx, int dy)
{
    //---------------------------------------------------------
    m_left += dx;
    m_right += dx;
    m_top += dy;
    m_bottom += dy;

}

void SelectRect::normalize()
{
    //---------------------------------------------------------
    if (m_left>m_right){
        int d = m_left;
        m_left = m_right;
        m_right = d;
    }
    if (m_top>m_bottom){
        int d = m_top;
        m_top = m_bottom;
        m_bottom = d;

    }
}

void SelectRect::backupRect()
{
    //--------------------------------------------------------
    m_leftBak   = m_left;
    m_rightBak  = m_right;
    m_topBak    = m_top;
    m_bottomBak = m_bottom;
}

void SelectRect::rectoreRect()
{
    m_left      = m_leftBak;
    m_right     = m_rightBak;
    m_top       = m_topBak;
    m_bottom    = m_bottomBak;
}

bool SelectRect::hitHandle(QPoint pt)
{
    //---------------------------------------------------------
    m_idHandle = -1;
    for (int i=0;i<CORNER_END;i++){
        if (m_Corners[i].contains(pt)){
            m_idHandle = i;
            return true;
        }
    }
    return false;
}

void SelectRect::setSelectedHandle(int x,int y)
{
    *(m_Corners[m_idHandle].m_x) = x;
    *(m_Corners[m_idHandle].m_y) = y;
}

int SelectRect::widthPix()
{
    return m_right-m_left+1;
}

int SelectRect::heightPix()
{
    return m_bottom-m_top+1;
}
