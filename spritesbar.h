#ifndef SPRITESBAR_H
#define SPRITESBAR_H

#include <QWidget>
#include <vector>
#include <memory>

class Sprite{
public:
    Sprite();
    ~Sprite();
    std::shared_ptr<QImage> m_image;
    QString     m_fileName;
    std::vector<std::shared_ptr<QImage>> m_states;

};

class spritesbar : public QWidget
{
    Q_OBJECT
public:
    explicit spritesbar(QWidget *parent = nullptr);
    ~spritesbar();

    int     m_cellSize;
    int     m_nbCells;
    std::vector<std::shared_ptr<Sprite>> m_tblSprites;
    int     m_idSelectedCell;

    int     mouseToIndex(QPoint pt);
    std::shared_ptr<QImage> getSelectedSprite();
    void    setSelectedSprite(std::shared_ptr<QImage> newImage);
    QString getSelectedSpriteFileName();
    void    newImage(int imgWidth, int imgHeight);
    void    openImage(QString fileName);
    void    saveImage();
    void    saveAsImage(const QString fileName, const char *fileFormat);

signals:
    void selectSpriteChanged(std::shared_ptr<QImage> sprite);

public slots:
    void    saveCurSpriteState();
    void    restoreCurSpriteState();
    void    refreshDisplay();
    void    setCurrentImage(std::shared_ptr<QImage> img);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

};

#endif // SPRITESBAR_H
