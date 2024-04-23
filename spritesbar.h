#ifndef SPRITESBAR_H
#define SPRITESBAR_H

#include <QWidget>
#include <vector>

class Sprite{
public:
    Sprite();
    ~Sprite();
    QImage      *m_image;
    QString     m_fileName;

};

class spritesbar : public QWidget
{
    Q_OBJECT
public:
    explicit spritesbar(QWidget *parent = nullptr);
    ~spritesbar();

    int     m_cellSize;
    int     m_nbCells;
    std::vector<Sprite *> m_tblSprites;
    int     m_idSelectedCell;

    int     mouseToIndex(QPoint pt);
    QImage* getSelectedSprite();
    QString getSelectedSpriteFileName();
    void    openImage(QString fileName);
    void    saveImage();
    void    saveAsImage(const QString fileName, const char *fileFormat);

signals:
    void selectSpriteChanged(QImage *sprite);

public slots:
    void refreshDisplay();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

};

#endif // SPRITESBAR_H
