#ifndef EDITMODE_H
#define EDITMODE_H
#include <QColor>
#include <QImage>
#include <QWidget>
#include <memory>
#include <vector>

class EditMode {
public:
  static QColor m_foreGroundColor;
  static QColor m_backGroundColor;
  static int m_pixWidth;
  static int m_pixHeight;
  static int m_cellSize;
  static float m_scale;
  static QTransform m_transform_scale;
  static QTransform m_transform_translate;

  static std::shared_ptr<QImage> m_image;
  static std::shared_ptr<QImage> m_imageBackup;
  static std::shared_ptr<QImage> m_imageCopy;

  static std::shared_ptr<QImage> m_imageStart;

//  static std::vector<std::shared_ptr<QImage>> m_states;


  EditMode();
  ~EditMode();

  bool mouseToPixel(int mx, int my, int &pixelX, int &pixelY);
  bool pixelToMouse(int pixelX, int pixelY, int &mouseX, int &mouseY);

//  void saveState();
//  void restoreState();
  void saveStartState();
  void restoreStartState();

  virtual void paintEvent(QWidget *w, QPainter &painter) = 0;
  virtual bool mousePressEvent(QWidget *w, QMouseEvent *event) = 0;
  virtual bool mouseReleaseEvent(QWidget *w, QMouseEvent *event) = 0;
  virtual bool mouseDoubleClickEvent(QWidget *w, QMouseEvent *event) = 0;
  virtual bool mouseMoveEvent(QWidget *w, QMouseEvent *event) = 0;
  virtual void resizeEvent(QWidget *w, QResizeEvent *event) = 0;
  virtual void initMode() = 0;

};

#endif // EDITMODE_H
