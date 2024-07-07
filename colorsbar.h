#ifndef COLORSBAR_H
#define COLORSBAR_H
#include <iostream>
#include <fstream>
#include <vector>
#include <QWidget>
#include <QRect>

class ColorRect : public QRect
{
public:
    explicit ColorRect();
    explicit ColorRect(QColor c);
    explicit ColorRect(int x, int y, int width, int height, QColor c);

    QColor color;

    void draw(QPainter *p, bool fFrame=false);

};

class colorsbar : public QWidget
{
    Q_OBJECT
public:

    enum
    {
        RGB_R_MASK = 0xFF << 24,
        RGB_G_MASK = 0xFF << 16,
        RGB_B_MASK = 0xFF << 8,
        RGB_A_MASK = 0xFF
    };

    explicit colorsbar(QWidget *parent = nullptr);
    ~colorsbar();

    int m_nbRows;
    int m_nbColumns;
    int m_cellSize;

    std::vector<ColorRect> tblColors;
    ColorRect foregroundColor;
    ColorRect backgroundColor;
    QColor    foreGroundColorBackup;

    bool        fMove;
    ColorRect   dragColor;

    void save(std::string pathName, std::string fileName);
    bool load(std::string pathName, std::string fileName);
    void drawCell(QPainter *p,int x,int y,int d,QColor c);
    bool mouse2Color(int x,int y,QColor &selColor);
    bool mouse2ColorIndex(int x,int y,int &idColor);

    unsigned char getRGBAlpha(unsigned int rgb);
    unsigned char getRGBRed(unsigned int rgb);
    unsigned char getRGBGreen(unsigned int rgb);
    unsigned char getRGBBlue(unsigned int rgb);
    unsigned int  RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a);

    QColor getForegroundColor();
    QColor getBackgroundColor();


signals:
    void foreGroundColorChanged(QColor c);
    void backGroundColorChanged(QColor c);

public slots:
    void setForeGroundColor(QColor newColor);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

};

#endif // COLORSBAR_H
