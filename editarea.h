#ifndef EDITAREA_H
#define EDITAREA_H

#include <QWidget>
#include <QColor>
#include <memory>
#include "editmode.h"
#include "editmodeselect.h"
#include "editmodepencil.h"
#include "editmoderectangle.h"
#include "editmodeellipse.h"
#include "editmodefill.h"

class editarea : public QWidget
{
    Q_OBJECT
public:

    explicit editarea(QWidget *parent = nullptr);
    ~editarea();

    int                 m_startX;
    int                 m_startY;
    int                 m_origin_dx_backup;
    int                 m_origin_dy_backup;
    int                 m_origin_dx;
    int                 m_origin_dy;

    QCursor             *m_pickColorCursor;
    EditMode            *m_editMode;
    EditModeSelect      m_editModeSelect;
    EditModePencil      m_editModePencil;
    EditModeRectangle   m_editModeRectangle;
    EditModeEllipse     m_editModeEllipse;
    EditModeFill        m_editModeFill;

    void drawEditImage(QPainter *p);

    void setSelectMode();
    void setPencilMode();
    void setRectangleMode();
    void setEllipseMode();
    void setFloodFillMode();
    void doUndo();
    std::shared_ptr<QImage> getEditImage();


signals:
    void saveImageState();
    void restoreImageState();
    void copyImageState();
    void editSpriteChanged();
    void pickImageColor(QColor c);
    void editImageChanged(std::shared_ptr<QImage> image);

public slots:
    void setForeGroundColor(QColor newColor);
    void setBackGroundColor(QColor newColor);
    void setEditSprite(std::shared_ptr<QImage> sprite);
    void cutSelectBox();
    void copySelectBox();
    void pasteSelectBox();


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

};

#endif // EDITAREA_H
