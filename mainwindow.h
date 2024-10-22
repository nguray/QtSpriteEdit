#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editarea.h"
#include "spritesbar.h"
#include "colorsbar.h"
#include <QStatusBar>
#include <QLabel>
#include <QToolBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    editarea    *m_editarea;
    spritesbar  *m_spritesbar;
    colorsbar   *m_colorsbar;

    QStatusBar  *m_statusBar;
    QToolBar    *m_toolBar;
    //QLabel      *m_status_msg;

    void        uncheckAllToolBarButtons();
    void        newSprite();
    void        open();
    void        save();
    void        saveAs();
    void        about();
    void        undo();


private slots:
    void setSelectMode();
    void setPencilMode();
    void setRectangleMode();
    void setEllipseMode();
    void setFillMode();
    void doPaste();

private:
    void createActions();
    void createMenus();
    void createToolBar();


    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    QAction *undoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;

    QAction *printAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QAction *selectModeAct;
    QAction *pencilModeAct;
    QAction *rectangleModeAct;
    QAction *ellipseModeAct;
    QAction *fillModeAct;

};
#endif // MAINWINDOW_H
