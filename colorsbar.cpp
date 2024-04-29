#include "colorsbar.h"
#include <QPainter>
#include <QColorDialog>
#include <QMouseEvent>
#include <sstream>

colorsbar::colorsbar(QWidget *parent)
    : QWidget{parent}
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumHeight(48);

    m_cellSize = 24;
    m_nbRows = 2;
    m_nbColumns = 20;

    //-- Fill colors table
    for (int i=0;i<m_nbRows;i++){
        for (int j=0;j<m_nbColumns;j++){
            m_tblColors.push_back(QColor(0x00,0x00,0x00,0x00));
        }
    }


    //--
    if (!load("","Default.pal")){
        int i = 0;
        m_tblColors[i++] = QColor(0x00,0x00,0x00,0x00);
        //m_tblColors[i++] = QColor(0x00,0x00,0x00,0xFF);
        m_tblColors[i++] = QColor(0xFF,0x00,0x00,0xFF);
        m_tblColors[i++] = QColor(0xFF,0xFF,0xFF,0xFF);
        m_tblColors[i++] = QColor(0x80,0x80,0x80,0xFF);
        m_tblColors[i++] = QColor(0xC0,0xC0,0xC0,0xFF);
        m_tblColors[i++] = QColor(0x80,0x00,0x00,0xFF);
        m_tblColors[i++] = QColor(0xFF,0x00,0x00,0xFF);
        m_tblColors[i++] = QColor(0x80,0x80,0x00,0xFF);
        m_tblColors[i++] = QColor(0xFF,0xFF,0x00,0xFF);
        m_tblColors[i++] = QColor(0x00,0x80,0x00,0xFF);
        m_tblColors[i++] = QColor(0x00,0xFF,0x00,0xFF);
        m_tblColors[i++] = QColor(0x00,0x80,0x80,0xFF);
        m_tblColors[i++] = QColor(0x00,0xFF,0xFF,0xFF);
        m_tblColors[i++] = QColor(0x00,0x00,0x80,0xFF);
        m_tblColors[i++] = QColor(0x00,0x00,0xFF,0xFF);
        m_tblColors[i++] = QColor(0x80,0x00,0x80,0xFF);
        m_tblColors[i++] = QColor(0xFF,0x00,0xFF,0xFF);
        m_foreGroundColor = m_tblColors[1];
        m_backGroundColor = m_tblColors[0];
    }


}

colorsbar::~colorsbar()
{
    save("","Default.pal");
    m_tblColors.clear();
}

QColor colorsbar::getForegroundColor()
{
    return m_foreGroundColor;
}

void colorsbar::setForeGroundColor(QColor newColor)
{
    //-----------------------------------------------------------
    m_foreGroundColor = newColor;
    update();
}

QColor colorsbar::getBackgroundColor()
{
    return m_backGroundColor;
}

bool colorsbar::mouse2Color(int x,int y,QColor &selColor)
{
    int     ic = 0;
    int     xLeft,xRight,yTop,yBottom;
    //-----------------------------------------------------------
    for (unsigned int i=0;i<m_nbRows;i++){
        yTop = i*m_cellSize;
        yBottom = yTop + m_cellSize;
        for(unsigned int j=0;j<m_nbColumns;j++){
            xLeft = j*m_cellSize + 2*m_cellSize;
            xRight = xLeft + m_cellSize;
            if ((x>xLeft)&&(x<xRight)&&(y>yTop)&&(y<yBottom)){
                selColor = m_tblColors[ic];
                return true;
            }
            ic++;
        }
    }
    return false;
}

bool colorsbar::mouse2ColorIndex(int x,int y,int &idColor)
{
    int     ic = 0;
    int     xLeft,xRight,yTop,yBottom;
    //-----------------------------------------------------------
    for (unsigned int i=0;i<m_nbRows;i++){
        yTop = i*m_cellSize;
        yBottom = yTop + m_cellSize;
        for(unsigned int j=0;j<m_nbColumns;j++){
            xLeft = j*m_cellSize + 2*m_cellSize;
            xRight = xLeft + m_cellSize;
            if ((x>xLeft)&&(x<xRight)&&(y>yTop)&&(y<yBottom)){
                idColor = ic;
                return true;
            }
            ic++;
        }
    }
    return false;
}

void colorsbar::mousePressEvent(QMouseEvent *event)
{
    QColor  selColor;
    QPoint lastPoint = event->pos();
    if (event->button()==Qt::LeftButton) {
        if (lastPoint.x()<(2*m_cellSize)){
            QColor d = m_foreGroundColor;
            m_foreGroundColor = m_backGroundColor;
            m_backGroundColor = d;
            emit foreGroundColorChanged(m_foreGroundColor);
            emit backGroundColorChanged(m_backGroundColor);
            update();
        }else{
            if (mouse2Color(lastPoint.x(), lastPoint.y(), selColor)){
                m_foreGroundColorBackup = m_foreGroundColor;
                m_foreGroundColor = selColor;
                emit foreGroundColorChanged(selColor);
                update();
            }
        }
    }else if (event->button()==Qt::RightButton){
        if (mouse2Color(lastPoint.x(), lastPoint.y(), selColor)){
            m_backGroundColor = selColor;
            emit backGroundColorChanged(selColor);
            update();
        }
    }

}

void colorsbar::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint pt = event->pos();
    if (event->button()==Qt::LeftButton) {
        int idColor;
        if (mouse2ColorIndex(pt.x(), pt.y(), idColor)){
            QColor curColor = m_tblColors[idColor];
            QColor color = QColorDialog::getColor( curColor, this );
            m_tblColors[idColor] = color;
            m_foreGroundColor = m_foreGroundColorBackup;
            emit foreGroundColorChanged(m_foreGroundColor);
            update();
        }


    }
}

void colorsbar::drawCell(QPainter *p,int x,int y,int s,QColor c)
{
    if (c.alpha()!=0){
        int d = s-1;
        p->fillRect(x,y,d,d,QBrush(c));
    }else{
        p->setBrush(Qt::NoBrush);
        p->setPen(QPen(QBrush(QColor(0,0,0)),0));
        int left = x+1;
        int top = y+1;
        int right = x + s-2;
        int bottom = y + s-2;
        p->drawLine(left,top,right,top);
        p->drawLine(right,top,right,bottom);
        p->drawLine(right,bottom,left,bottom);
        p->drawLine(left,bottom,left,top);
        p->drawLine(left,top,right,bottom);
        p->drawLine(left,bottom,right,top);
    }

}

void colorsbar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,false);

    auto w = width();
    auto h = height();

    if (h<w){
        m_cellSize = h/m_nbRows;
    }else{
        m_cellSize = w/m_nbColumns;
    }

    int x, y;
    for (int j=0;j<m_nbRows;j++){
        y = j*m_cellSize;
        for(int i=0;i<m_nbColumns;i++){
            x = (i+2)*m_cellSize;
            QColor c = m_tblColors[j*m_nbColumns+i];
            drawCell( &painter, x, y, m_cellSize, c);
        }
    }

    //--
    drawCell( &painter, 0, 0, 2*m_cellSize, m_backGroundColor);

    //--
    drawCell( &painter, 2, 2, 1.3*m_cellSize, m_foreGroundColor);

}

unsigned char colorsbar::getRGBAlpha(unsigned int rgb)
{
    return (rgb & RGB_A_MASK);
}

unsigned char colorsbar::getRGBRed(unsigned int rgb)
{
    return (rgb & RGB_R_MASK) >> 24;
}

unsigned char colorsbar::getRGBGreen(unsigned int rgb)
{
    return (rgb & RGB_G_MASK) >> 16;
}

unsigned char colorsbar::getRGBBlue(unsigned int rgb)
{
    return (rgb & RGB_B_MASK) >> 8;
}

unsigned int colorsbar::RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
    return (b << 8) | (g << 16) | (r << 24) | (a);
}

void colorsbar::save(std::string pathName, std::string fileName)
{
    std::string fullPathName;
    //-------------------------------------------------
    if (pathName!=""){
        fullPathName = pathName + "/" + fileName;
    }else{
        fullPathName = fileName;
    }
    std::ofstream   f(fullPathName);
    if (f.is_open()){
        //--
        QColor  qc;
        unsigned int ic;
        ic = RGBA(
            m_foreGroundColor.red(),
            m_foreGroundColor.green(),
            m_foreGroundColor.blue(),
            m_foreGroundColor.alpha());
        f << "FOREGROUND " << ic << std::endl;
        //--
        ic = RGBA(
            m_backGroundColor.red(),
            m_backGroundColor.green(),
            m_backGroundColor.blue(),
            m_backGroundColor.alpha());
        f << "BACKGROUND " << ic << std::endl;
        //--
        for(int i=0;i<m_nbRows*m_nbColumns;i++){
            qc = m_tblColors[i];
            ic = RGBA(
                qc.red(),
                qc.green(),
                qc.blue(),
                qc.alpha());
            f << ic << std::endl;
        }
        f.close();
    }

}

bool colorsbar::load(std::string pathName, std::string fileName)
{
    unsigned int ic;
    std::string strline;
    std::string strWord;
    std::string fullPathName;
    //-------------------------------------------------
    if (pathName!=""){
        fullPathName = pathName + "/" + fileName;
    }else{
        fullPathName = fileName;
    }
    std::ifstream   f(fullPathName);
    if (f.is_open()){
        int  i = 0;
        //--
        while(!f.eof()){
            std::getline(f,strline);
            std::stringstream ss(strline);
            ss >> strWord;
            if (strWord=="FOREGROUND"){
                ss >> strWord;
                ic = std::atoi(strWord.c_str());
                m_foreGroundColor = QColor(
                    getRGBRed(ic),
                    getRGBGreen(ic),
                    getRGBBlue(ic),
                    getRGBAlpha(ic)
                );
            }else if (strWord=="BACKGROUND"){
                ss >> strWord;
                ic = std::atoi(strWord.c_str());
                m_backGroundColor = QColor(
                    getRGBRed(ic),
                    getRGBGreen(ic),
                    getRGBBlue(ic),
                    getRGBAlpha(ic)
                    );
            }else{
                ic = std::atoi(strWord.c_str());
                if (i<m_nbRows*m_nbColumns){
                    m_tblColors[i++] = QColor(
                        getRGBRed(ic),
                        getRGBGreen(ic),
                        getRGBBlue(ic),
                        getRGBAlpha(ic)
                        );
                }else{
                    break;
                }
            }
        }
        f.close();
        return true;
    }
    return false;
}

