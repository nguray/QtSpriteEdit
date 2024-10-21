#include "editmode.h"

#include <QColor>
#include <QPainter>


QColor EditMode::m_foreGroundColor = QColor(0, 0, 0);
QColor EditMode::m_backGroundColor = QColor(0, 0, 0, 0);
int EditMode::m_pixWidth = 32;
int EditMode::m_pixHeight = 32;
int EditMode::m_cellSize = 24;
float EditMode::m_scale = 1.0f;
QTransform EditMode::m_transform_scale = QTransform::fromScale(1.0f, 1.0f);
QTransform EditMode::m_transform_translate =
    QTransform::fromTranslate(0.0f, 0.0f);
std::shared_ptr<QImage> EditMode::m_image = std::make_shared<QImage>(
    EditMode::m_pixWidth, EditMode::m_pixHeight, QImage::Format_ARGB32);
std::shared_ptr<QImage> EditMode::m_imageBackup = std::make_shared<QImage>(
    EditMode::m_pixWidth, EditMode::m_pixHeight, QImage::Format_ARGB32);
std::shared_ptr<QImage> EditMode::m_imageCopy = std::make_shared<QImage>(
    EditMode::m_pixWidth, EditMode::m_pixHeight, QImage::Format_ARGB32);

EditMode::EditMode() {}

EditMode::~EditMode() {}

bool EditMode::mouseToPixel(int mx, int my, int &pixelX, int &pixelY)
/*----------------------------------------------------------------------------*\
    Description :


    Date de création : 20-04-2024                       Raymond NGUYEN THANH
\*----------------------------------------------------------------------------*/
{
  //---------------------------------------------------------
  pixelX = mx / m_cellSize;
  pixelY = my / m_cellSize;
  return (pixelX >= 0) && (pixelX < m_pixWidth) && (pixelY >= 0) &&
         (pixelY < m_pixHeight);
}

bool EditMode::pixelToMouse(int pixelX, int pixelY, int &mouseX, int &mouseY)
/*----------------------------------------------------------------------------*\
    Description :


    Date de création : 20-04-2024                       Raymond NGUYEN THANH
\*----------------------------------------------------------------------------*/
{
  //---------------------------------------------------------
  mouseX = pixelX * m_cellSize;
  mouseY = pixelY * m_cellSize;
  return true;
}

void EditMode::backup() {
  if (m_imageBackup && m_image) {
    m_imageBackup->fill(QColor(0, 0, 0, 0));
    // for(int y=0;y<m_image->height();y++){
    //     for(int x=0;x<m_image->width();x++){
    //         QColor c = m_image->pixelColor(x,y);
    //         m_imageBackup->setPixelColor(x,y,c);
    //     }
    // }
    QPainter painter(m_imageBackup.get());
    painter.drawImage(QPoint(0, 0), *m_image);
    painter.end();
  }
}

void EditMode::restore() {
  if (m_imageBackup && m_image) {
    m_image->fill(QColor(0, 0, 0, 0));
    // for(int y=0;y<m_image->height();y++){
    //     for(int x=0;x<m_image->width();x++){
    //         QColor c = m_imageBackup->pixelColor(x,y);
    //         m_image->setPixelColor(x,y,c);
    //     }
    // }
    QPainter painter(m_image.get());
    painter.drawImage(QPoint(0, 0), *m_imageBackup);
    painter.end();
  }
}
