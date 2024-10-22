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

std::vector<std::shared_ptr<QImage>> EditMode::m_states;

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

void EditMode::saveState()
{
    //---------------------------------------------------------
    auto img = std::make_shared<QImage>(
        EditMode::m_pixWidth, EditMode::m_pixHeight, QImage::Format_ARGB32);
    img->fill(QColor(0, 0, 0, 0));
    QPainter painter(img.get());
    painter.drawImage(QPoint(0, 0), *m_image);
    painter.end();
    m_states.push_back(img);

}

void EditMode::restoreState()
{
    //---------------------------------------------------------
    if (m_states.size()){

        auto oldSprite = m_states.back();
        m_states.pop_back();

        auto oldWidth = oldSprite->width();
        auto oldHeight = oldSprite->height();
        auto curWidth = m_image->width();
        auto curHeight = m_image->height();

        if ((oldWidth!=curWidth)||(oldHeight!=curHeight)){
            EditMode::m_pixWidth = oldWidth;
            EditMode::m_pixHeight = oldHeight;
            m_image = std::make_shared<QImage>(
                EditMode::m_pixWidth, EditMode::m_pixHeight, QImage::Format_ARGB32);
        }
        m_image->fill(QColor(0, 0, 0, 0));
        QPainter painter(m_image.get());
        painter.drawImage(QPoint(0, 0), *oldSprite);
        painter.end();

    }

}

void EditMode::restoreStartState()
{
    if (m_states.size()){

        auto lastImageOnStack = m_states.back();
        m_image->fill(QColor(0, 0, 0, 0));
        QPainter painter(m_image.get());
        painter.drawImage(QPoint(0, 0), *lastImageOnStack);
        painter.end();

    }

}

