#include "colorsbar.h"
#include <QColor>
#include <QColorDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <sstream>

ColorRect::ColorRect() : QRect(), color(QColor(0x00, 0x00, 0x00, 0x00)) {}

ColorRect::ColorRect(QColor c) : QRect(), color(c) {}

ColorRect::ColorRect(int x, int y, int width, int height, QColor c)
    : QRect(x, y, width, height), color(c) {}

void ColorRect::draw(QPainter *p, bool fFrame) {
  //--------------------------------------------------
  int s = width();
  if (color.alpha() != 0) {
    int d = s - 1;
    p->fillRect(x(), y(), d, d, QBrush(color));
    if (fFrame) {
      p->setBrush(Qt::NoBrush);
      unsigned char r = 255 - color.red();
      unsigned char g = 255 - color.green();
      unsigned char b = 255 - color.blue();
      p->setPen(QPen(QBrush(QColor(r, g, b)), 0));
      int left = x();
      int top = y();
      int right = x() + s - 1;
      int bottom = y() + s - 1;
      p->drawLine(left, top, right, top);
      p->drawLine(right, top, right, bottom);
      p->drawLine(right, bottom, left, bottom);
      p->drawLine(left, bottom, left, top);
    }
  } else {
    p->setBrush(Qt::NoBrush);
    p->setPen(QPen(QBrush(QColor(0, 0, 0)), 0));
    int left = x() + 1;
    int top = y() + 1;
    int right = x() + s - 2;
    int bottom = y() + s - 2;
    p->drawLine(left, top, right, top);
    p->drawLine(right, top, right, bottom);
    p->drawLine(right, bottom, left, bottom);
    p->drawLine(left, bottom, left, top);
    p->drawLine(left, top, right, bottom);
    p->drawLine(left, bottom, right, top);
  }
}

colorsbar::colorsbar(QWidget *parent) : QWidget{parent}, fMove(false) {
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  setMinimumHeight(48);

  m_cellSize = 24;
  m_nbRows = 2;
  m_nbColumns = 20;

  //-- Fill colors table
  for (int i = 0; i < m_nbRows; i++) {
    for (int j = 0; j < m_nbColumns; j++) {
      tblColors.push_back(ColorRect());
    }
  }

  //--
  if (!load("", "Default.pal")) {
    int i = 0;
    tblColors[i++].color = QColor(0x00, 0x00, 0x00, 0x00);
    // tblColors[i++].color = QColor(0x00,0x00,0x00,0xFF);
    tblColors[i++].color = QColor(0xFF, 0x00, 0x00, 0xFF);
    tblColors[i++].color = QColor(0xFF, 0xFF, 0xFF, 0xFF);
    tblColors[i++].color = QColor(0x80, 0x80, 0x80, 0xFF);
    tblColors[i++].color = QColor(0xC0, 0xC0, 0xC0, 0xFF);
    tblColors[i++].color = QColor(0x80, 0x00, 0x00, 0xFF);
    tblColors[i++].color = QColor(0xFF, 0x00, 0x00, 0xFF);
    tblColors[i++].color = QColor(0x80, 0x80, 0x00, 0xFF);
    tblColors[i++].color = QColor(0xFF, 0xFF, 0x00, 0xFF);
    tblColors[i++].color = QColor(0x00, 0x80, 0x00, 0xFF);
    tblColors[i++].color = QColor(0x00, 0xFF, 0x00, 0xFF);
    tblColors[i++].color = QColor(0x00, 0x80, 0x80, 0xFF);
    tblColors[i++].color = QColor(0x00, 0xFF, 0xFF, 0xFF);
    tblColors[i++].color = QColor(0x00, 0x00, 0x80, 0xFF);
    tblColors[i++].color = QColor(0x00, 0x00, 0xFF, 0xFF);
    tblColors[i++].color = QColor(0x80, 0x00, 0x80, 0xFF);
    tblColors[i++].color = QColor(0xFF, 0x00, 0xFF, 0xFF);

    foregroundColor.color = tblColors[1].color;
    backgroundColor.color = tblColors[0].color;
  }
}

colorsbar::~colorsbar() {
  save("", "Default.pal");
  tblColors.clear();
}

QColor colorsbar::getForegroundColor() { return foregroundColor.color; }

void colorsbar::setForeGroundColor(QColor newColor) {
  //-----------------------------------------------------------
  foregroundColor.color = newColor;
  update();
}

QColor colorsbar::getBackgroundColor() { return backgroundColor.color; }

bool colorsbar::mouse2Color(int x, int y, QColor &selColor) {
  //-----------------------------------------------------------
  for (auto &cr : tblColors) {
    if (cr.contains(x, y)) {
      selColor = cr.color;
      return true;
    }
  }
  return false;
}

bool colorsbar::mouse2ColorIndex(int x, int y, int &idColor) {
  int ic = 0;
  //-----------------------------------------------------------
  for (auto &cr : tblColors) {
    if (cr.contains(x, y)) {
      idColor = ic;
      return true;
    }
    ic++;
  }
  return false;
}

void colorsbar::mousePressEvent(QMouseEvent *event) {
  //-----------------------------------------------------------
  QColor selColor;
  QPoint lastPoint = event->pos();
  if (event->button() == Qt::LeftButton) {
    if (foregroundColor.contains(lastPoint.x(), lastPoint.y())) {
      int offSet = m_cellSize / 2;
      fMove = true;
      dragColor.color = foregroundColor.color;
      dragColor.setRect(lastPoint.x() - offSet, lastPoint.y() - offSet,
                        m_cellSize, m_cellSize);
    } else if (backgroundColor.contains(lastPoint.x(), lastPoint.y())) {
      QColor d = foregroundColor.color;
      foregroundColor.color = backgroundColor.color;
      backgroundColor.color = d;
      emit foreGroundColorChanged(foregroundColor.color);
      emit backGroundColorChanged(backgroundColor.color);
      update();
    } else {
      if (mouse2Color(lastPoint.x(), lastPoint.y(), selColor)) {
        foreGroundColorBackup = foregroundColor.color;
        foregroundColor.color = selColor;
        emit foreGroundColorChanged(selColor);
        update();
      }
    }
  } else if (event->button() == Qt::RightButton) {
    if (mouse2Color(lastPoint.x(), lastPoint.y(), selColor)) {
      backgroundColor.color = selColor;
      emit backGroundColorChanged(selColor);
      update();
    }
  }
}

void colorsbar::mouseReleaseEvent(QMouseEvent *event) {
  QPoint pt = event->pos();
  //----------------------------------------------------------
  if (fMove) {
    int idColor;
    fMove = false;
    dragColor.setRect(-1, -1, 0, 0);
    if (mouse2ColorIndex(pt.x(), pt.y(), idColor)) {
      tblColors[idColor].color = dragColor.color;
    }
    update();
  }
}

void colorsbar::mouseDoubleClickEvent(QMouseEvent *event) {
  //-----------------------------------------------------------
  QPoint pt = event->pos();
  if (event->button() == Qt::LeftButton) {
    int idColor;
    if (mouse2ColorIndex(pt.x(), pt.y(), idColor)) {
      QColor curColor = tblColors[idColor].color;
      QColor color = QColorDialog::getColor(curColor, this);
      tblColors[idColor].color = color;
      foregroundColor.color = foreGroundColorBackup;
      emit foreGroundColorChanged(foregroundColor.color);
      update();
    }
  }
}

void colorsbar::mouseMoveEvent(QMouseEvent *event) {
  //-----------------------------------------------------------
  QPoint lastPoint = event->pos();
  if (fMove) {
    int offSet = m_cellSize / 2;
    dragColor.setRect(lastPoint.x() - offSet, lastPoint.y() - offSet,
                      m_cellSize, m_cellSize);
    update();
  }
}

void colorsbar::paintEvent(QPaintEvent *event) {
  //-----------------------------------------------------------
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, false);

  auto w = width();
  auto h = height();

  if (h < w) {
    m_cellSize = h / m_nbRows;
  } else {
    m_cellSize = w / m_nbColumns;
  }

  int x, y;
  for (int j = 0; j < m_nbRows; j++) {
    y = j * m_cellSize;
    for (int i = 0; i < m_nbColumns; i++) {
      x = (i + 2) * m_cellSize;
      tblColors[j * m_nbColumns + i].setRect(x, y, m_cellSize, m_cellSize);
      tblColors[j * m_nbColumns + i].draw(&painter);
    }
  }

  //--
  backgroundColor.setRect(0, 0, 2 * m_cellSize, 2 * m_cellSize);
  backgroundColor.draw(&painter);

  //--
  foregroundColor.setRect(2, 2, 1.3 * m_cellSize, 1.3 * m_cellSize);
  foregroundColor.draw(&painter);

  //--
  if (!dragColor.isNull()) {
    dragColor.draw(&painter, true);
  }
}

unsigned char colorsbar::getRGBAlpha(unsigned int rgb) {
  //-----------------------------------------------------------
  return (rgb & RGB_A_MASK);
}

unsigned char colorsbar::getRGBRed(unsigned int rgb) {
  //-----------------------------------------------------------
  return (rgb & RGB_R_MASK) >> 24;
}

unsigned char colorsbar::getRGBGreen(unsigned int rgb) {
  //-----------------------------------------------------------
  return (rgb & RGB_G_MASK) >> 16;
}

unsigned char colorsbar::getRGBBlue(unsigned int rgb) {
  //-----------------------------------------------------------
  return (rgb & RGB_B_MASK) >> 8;
}

unsigned int colorsbar::RGBA(unsigned char r, unsigned char g, unsigned char b,
                             unsigned char a) {
  //-----------------------------------------------------------
  return (b << 8) | (g << 16) | (r << 24) | (a);
}

void colorsbar::save(std::string pathName, std::string fileName) {
  std::string fullPathName;
  //-------------------------------------------------
  if (pathName != "") {
    fullPathName = pathName + "/" + fileName;
  } else {
    fullPathName = fileName;
  }
  std::ofstream f(fullPathName);
  if (f.is_open()) {
    //--
    QColor qc;
    unsigned int ic;
    qc = foregroundColor.color;
    ic = RGBA(qc.red(), qc.green(), qc.blue(), qc.alpha());
    f << "FOREGROUND " << ic << std::endl;
    //--
    qc = backgroundColor.color;
    ic = RGBA(qc.red(), qc.green(), qc.blue(), qc.alpha());
    f << "BACKGROUND " << ic << std::endl;
    //--
    for (int i = 0; i < m_nbRows * m_nbColumns; i++) {
      qc = tblColors[i].color;
      ic = RGBA(qc.red(), qc.green(), qc.blue(), qc.alpha());
      f << ic << std::endl;
    }
    f.close();
  }
}

bool colorsbar::load(std::string pathName, std::string fileName) {
  unsigned int ic;
  std::string strline;
  std::string strWord;
  std::string fullPathName;
  //-------------------------------------------------
  if (pathName != "") {
    fullPathName = pathName + "/" + fileName;
  } else {
    fullPathName = fileName;
  }

  std::ifstream f(fullPathName);
  if (f.is_open()) {
    int i = 0;
    //--
    while (!f.eof()) {
      std::getline(f, strline);
      std::stringstream ss(strline);
      ss >> strWord;
      if (strWord == "FOREGROUND") {
        ss >> strWord;
        ic = std::atoi(strWord.c_str());
        foregroundColor.color = QColor(getRGBRed(ic), getRGBGreen(ic),
                                       getRGBBlue(ic), getRGBAlpha(ic));
      } else if (strWord == "BACKGROUND") {
        ss >> strWord;
        ic = std::atoi(strWord.c_str());
        backgroundColor.color = QColor(getRGBRed(ic), getRGBGreen(ic),
                                       getRGBBlue(ic), getRGBAlpha(ic));
      } else {
        ic = std::atoi(strWord.c_str());
        if (i < m_nbRows * m_nbColumns) {
          tblColors[i].color = QColor(getRGBRed(ic), getRGBGreen(ic),
                                      getRGBBlue(ic), getRGBAlpha(ic));
          i++;
        } else {
          break;
        }
      }
    }
    f.close();
    return true;
  }
  return false;
}
