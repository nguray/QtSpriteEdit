#ifndef EDITAREA_H
#define EDITAREA_H

#include <QWidget>
#include <QColor>
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


signals:
    void editSpriteChanged();

public slots:
    void setForeGroundColor(QColor newColor);
    void setBackGroundColor(QColor newColor);
    void setEditSprite(QImage *sprite);
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
    void resizeEvent(QResizeEvent *event);

};

#endif // EDITAREA_H
