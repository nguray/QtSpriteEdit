#include "spritesbar.h"
#include <QPainter>
#include <QMouseEvent>
#include "newsprite2dialog.h"
#include <iostream>


Sprite::Sprite():m_image(NULL),m_fileName("")
{

}

Sprite::~Sprite()
{
}

spritesbar::spritesbar(QWidget *parent)
    : QWidget{parent}
{
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    setMinimumWidth(65);
    m_cellSize = 64;
    m_nbCells = 8;
    m_idSelectedCell = 0;

    for (int i=0;i<m_nbCells;i++){
        auto spr = std::make_shared<Sprite>();
        if (spr){
            spr->m_image = std::make_shared<QImage>(32,32,QImage::Format_ARGB32);
            if (spr->m_image!=NULL){
                spr->m_image->fill(QColor(0,0,0,0));
                m_tblSprites.push_back(spr);
            }
        }
    }

}

spritesbar::~spritesbar()
{

}

void spritesbar::refreshDisplay()
{
    update();
}

int spritesbar::mouseToIndex(QPoint pt)
{
    int id = pt.y()/m_cellSize;
    if ((id>=0)&&(id<m_nbCells)){
        return id;
    }
    return -1;
}

std::shared_ptr<QImage> spritesbar::getSelectedSprite()
{
    if ((m_idSelectedCell>=0)&&(m_idSelectedCell<m_nbCells)){
        return m_tblSprites[m_idSelectedCell]->m_image;
    }
    return NULL;
}

void spritesbar::setSelectedSprite(std::shared_ptr<QImage> newImage)
{
    if ((m_idSelectedCell>=0)&&(m_idSelectedCell<m_nbCells)){
        m_tblSprites[m_idSelectedCell]->m_image = newImage;
        update();
    }
}


QString spritesbar::getSelectedSpriteFileName()
{
    if ((m_idSelectedCell>=0)&&(m_idSelectedCell<m_nbCells)){
        return m_tblSprites[m_idSelectedCell]->m_fileName;
    }
    return "";
}

void spritesbar::mousePressEvent(QMouseEvent *event)
{
    QPoint pt = event->pos();
    if (event->button()==Qt::LeftButton) {

        // NewSprite2Dialog dlg(this);
        // auto res = dlg.exec();

        // if (res==QDialog::Accepted){
        //     auto w = dlg.m_width;
        //     auto h = dlg.m_height;
        //     std::cout << "width: " << w << std::endl;
        // }

        int id = mouseToIndex(pt);
        if (id!=-1){
            m_idSelectedCell = id;
            if (m_tblSprites[id]){
                emit selectSpriteChanged(m_tblSprites[id]->m_image);
                update();
            }
        }
    }
}

void spritesbar::newImage(int imgWidth, int imgHeight)
{
    if ((m_idSelectedCell>=0)&&(m_idSelectedCell<m_nbCells)){
        auto img = std::make_shared<QImage>(imgWidth, imgHeight, QImage::Format_ARGB32);
        if (img){
            img->fill(QColor(0,0,0,0));
            m_tblSprites[m_idSelectedCell]->m_image = img;
            emit selectSpriteChanged(img);
            update();
        }
    }
}

void spritesbar::openImage(QString fileName)
{
    //-------------------------------------------------------
    auto spr = getSelectedSprite();
    if (spr ){
        spr->load(fileName);
        m_tblSprites[m_idSelectedCell]->m_fileName = fileName;
        emit selectSpriteChanged(spr);
        update();
    }
}

void spritesbar::saveImage()
{
    auto sprite = getSelectedSprite();
    if (sprite ){
        sprite->save(m_tblSprites[m_idSelectedCell]->m_fileName);
    }
}

void spritesbar::saveAsImage(const QString fileName, const char *fileFormat)
{
    //-------------------------------------------------------
    auto sprite = getSelectedSprite();
    if (sprite ){
        if (sprite->save(fileName,fileFormat)){
            m_tblSprites[m_idSelectedCell]->m_fileName = fileName;
        }
    }

}


void spritesbar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,false);

    painter.setPen(QPen(QBrush(QColor(150,150,150)),0));
    int id = 0;
    int y = 0;
    painter.drawLine(0,y,m_cellSize,y);
    for (auto spr : m_tblSprites){
        if (spr!=NULL){
            int w = spr->m_image->width();
            int h = spr->m_image->height();
            painter.drawImage(QPoint(m_cellSize/2-w/2,y+m_cellSize/2-h/2),*(spr->m_image));
        }
        if (id==m_idSelectedCell){
            int top = y + 1;
            int left = 1;
            int bottom = top + m_cellSize - 2;
            int right = left + m_cellSize - 2;
            int offSet = 8;
            painter.setPen(QPen(QBrush(QColor(255,0,0)),0));
            // Top Left
            painter.drawLine(left,top,left+offSet,top);
            painter.drawLine(left,top,left,top+offSet);
            // Top Right
            painter.drawLine(right-offSet,top,right,top);
            painter.drawLine(right,top,right,top+offSet);
            // Bottom Left
            painter.drawLine(left,bottom-offSet,left,bottom);
            painter.drawLine(left,bottom,left+offSet,bottom);
            // Bottom Right
            painter.drawLine(right-offSet,bottom,right,bottom);
            painter.drawLine(right,bottom,right,bottom-offSet);

        }
        y += m_cellSize;
        id++;
        painter.setPen(QPen(QBrush(QColor(150,150,150)),0));
        painter.drawLine(0,y,m_cellSize,y);

    }

    painter.drawLine(0,0,0,y);
    painter.drawLine(m_cellSize,0,m_cellSize,y);


}
