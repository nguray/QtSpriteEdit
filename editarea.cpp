#include "editarea.h"
#include "editmode.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QRect>

editarea::editarea(QWidget *parent)
    : QWidget{parent}, m_startX(0), m_startY(0), m_origin_dx(0),
      m_origin_dy(0) {
  m_editMode->m_foreGroundColor = QColor(255, 0, 0);
  m_editMode->m_backGroundColor = QColor(0, 0, 0, 0);
  m_editMode->m_pixWidth = 32;
  m_editMode->m_pixHeight = 32;
  m_editMode->m_cellSize = 24;
  m_editMode = &m_editModePencil;
  m_pickColorCursor = new QCursor(QPixmap(":res/PickColor.png"), 6, 23);
}

editarea::~editarea() {
  if (m_pickColorCursor) {
    delete m_pickColorCursor;
  }
}

void editarea::setEditSprite(std::shared_ptr<QImage> sprite) {
  if (sprite) {
    m_editMode->m_pixWidth = sprite->width();
    m_editMode->m_pixHeight = sprite->height();
    m_editMode->m_image = sprite;
    m_editMode->m_imageBackup = std::make_shared<QImage>(sprite->width(), sprite->height(), sprite->format());
    m_editMode->m_imageBackup->fill(QColor(0, 0, 0, 0));
    update();
  }
}

void editarea::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  auto w = width();
  auto h = height();

  if (w > h) {
    if (m_editMode->m_pixHeight > m_editMode->m_pixWidth) {
      m_editMode->m_cellSize = h / m_editMode->m_pixHeight;
    } else {
      m_editMode->m_cellSize = h / m_editMode->m_pixWidth;
    }
  } else {
    if (m_editMode->m_pixHeight > m_editMode->m_pixWidth) {
      m_editMode->m_cellSize = w / m_editMode->m_pixHeight;
    } else {
      m_editMode->m_cellSize = w / m_editMode->m_pixWidth;
    }
  }

  painter.setRenderHint(QPainter::Antialiasing, false);
  painter.setClipRegion(
      QRegion(0, 0, m_editMode->m_pixWidth * m_editMode->m_cellSize + 2,
              m_editMode->m_pixHeight * m_editMode->m_cellSize + 2,
              QRegion::Rectangle));

  painter.setTransform(
      QTransform::fromScale(m_editMode->m_scale, m_editMode->m_scale));
  painter.setTransform(QTransform::fromTranslate(m_origin_dx, m_origin_dy),
                       true);

  int x, y;
  painter.setPen(QPen(QBrush(QColor(50, 50, 50)), 0));
  for (int i = 0; i <= m_editMode->m_pixHeight; i++) {
    y = i * m_editMode->m_cellSize;
    for (int j = 0; j <= m_editMode->m_pixWidth; j++) {
      x = j * m_editMode->m_cellSize;
      painter.drawPoint(x, y);
    }
  }

  //--
  drawEditImage(&painter);

  //--
  m_editMode->paintEvent(this, painter);

  // m_selectBox.draw(&painter);

  // painter.fillRect(10, 10, 50, 50, QBrush(QColor(100,0,0,10)));
  // painter.drawImage(QPoint(0,0),*m_image);
}

void editarea::drawEditImage(QPainter *p) {
  int x, y;
  //---------------------------------------------------------
  if (m_editMode->m_image) {
    int d = m_editMode->m_cellSize - 2;
    for (int i = 0; i < m_editMode->m_pixHeight; i++) {
      y = i * m_editMode->m_cellSize;
      for (int j = 0; j < m_editMode->m_pixWidth; j++) {
        QColor c = m_editMode->m_image->pixelColor(j, i);
        x = j * m_editMode->m_cellSize;
        p->fillRect(x + 1, y + 1, d, d, QBrush(c));
      }
    }
  }
}

void editarea::setForeGroundColor(QColor newColor) {
  m_editMode->m_foreGroundColor = newColor;
}

void editarea::setBackGroundColor(QColor newColor) {
  m_editMode->m_backGroundColor = newColor;
}

void editarea::setSelectMode() {
  m_editModeSelect.m_selectBox.reset();
  m_editMode = &m_editModeSelect;
  update();
}

void editarea::setPencilMode() {
  m_editMode = &m_editModePencil;
  update();
}

void editarea::setRectangleMode() {
  m_editModeRectangle.m_selectBox.reset();
  m_editMode = &m_editModeRectangle;
  update();
}

void editarea::setEllipseMode() {
  m_editModeEllipse.m_selectBox.reset();
  m_editMode = &m_editModeEllipse;
  update();
}

void editarea::setFloodFillMode() {
  m_editMode = &m_editModeFill;
  update();
}

void editarea::cutSelectBox() {
  //-----------------------------------
  if (m_editMode == &m_editModeSelect) {
    if (m_editModeSelect.m_selectBox.width() &&
        m_editModeSelect.m_selectBox.height()) {
      m_editModeSelect.cutSelectBox();
      emit editSpriteChanged();
      update();
    }
  }
}

void editarea::copySelectBox() {
  //-----------------------------------
  if (m_editMode == &m_editModeSelect) {
    if (m_editModeSelect.m_selectBox.width() &&
        m_editModeSelect.m_selectBox.height()) {
      m_editModeSelect.copySelectBox();
      update();
    }
  }
}

void editarea::pasteSelectBox() {
  //-----------------------------------
  if (m_editMode != &m_editModeSelect) {
    setSelectMode();
  }
  m_editModeSelect.pasteSelectBox();
  m_editModeSelect.m_selectBox.updateRect(m_editMode->m_cellSize);
  emit editSpriteChanged();
  update();
}

void editarea::doUndo() {
    //-----------------------------------
    m_editMode->restoreState();
    emit editImageChanged(m_editMode->m_image);
    update();
}

std::shared_ptr<QImage> editarea::getEditImage()
{
    //-----------------------------------
    return m_editMode->m_image;
}

void editarea::mousePressEvent(QMouseEvent *event) {
  setFocus();
  if (event->button() == Qt::MiddleButton) {
    m_startX = event->pos().x();
    m_startY = event->pos().y();
    m_origin_dx_backup = m_origin_dx;
    m_origin_dy_backup = m_origin_dy;

  } else if ((event->button() == Qt::LeftButton) &&
             (QApplication::keyboardModifiers() & Qt::ShiftModifier)) {
    //--
    int pixelX, pixelY;
    QTransform trans =
        m_editMode->m_transform_scale * m_editMode->m_transform_translate;
    QPoint pt = trans.map(event->pos());
    if (m_editMode->mouseToPixel(pt.x(), pt.y(), pixelX, pixelY)) {
      QColor pickColor = m_editMode->m_image->pixelColor(pixelX, pixelY);
      m_editMode->m_foreGroundColor = pickColor;
      emit pickImageColor(pickColor);
      // update();
      // event->accept();
    }
  } else {
    m_editMode->mousePressEvent(this, event);
  }
}

void editarea::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::MiddleButton) {
    m_editMode->m_transform_translate =
        QTransform::fromTranslate(m_origin_dx, m_origin_dy).inverted();
  }
  m_editMode->mouseReleaseEvent(this, event);
}

void editarea::mouseDoubleClickEvent(QMouseEvent *event) {
  m_editMode->mouseDoubleClickEvent(this, event);
}

void editarea::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::MiddleButton) {
    int dx = event->pos().x() - m_startX;
    int dy = event->pos().y() - m_startY;
    if (dx || dy) {
      m_origin_dx = m_origin_dx_backup;
      m_origin_dy = m_origin_dy_backup;
      m_origin_dx += dx;
      m_origin_dy += dy;
      update();
    }
  }
  m_editMode->mouseMoveEvent(this, event);
}

void editarea::wheelEvent(QWheelEvent *event) {
  QPoint numDegrees = event->angleDelta() / 8;

  if (numDegrees.y() > 0) {
    m_editMode->m_scale += 0.05f;
  } else {
    m_editMode->m_scale -= 0.05f;
  }
  m_editMode->m_transform_scale =
      QTransform::fromScale(m_editMode->m_scale, m_editMode->m_scale)
          .inverted();
  update();
  event->accept();
}

void editarea::resizeEvent(QResizeEvent *event) {
  m_editMode->resizeEvent(this, event);
}

void editarea::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Shift) {
    //--
    setCursor(*m_pickColorCursor);
  }
}

void editarea::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Shift) {
    //--
    setCursor(Qt::ArrowCursor);
  }
}
