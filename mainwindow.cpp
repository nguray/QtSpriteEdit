#include "mainwindow.h"
#include "aboutdlg.h"
#include "newspritedlg.h"
#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QString>
#include <QStyle>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}
{

  m_statusBar = this->statusBar();

  // m_status_msg = new QLabel( tr("  Test  ") );
  // m_status_msg->setMinimumSize( m_status_msg->sizeHint() );
  // m_status_msg->setAlignment( Qt::AlignCenter );
  // m_statusBar->addWidget( m_status_msg );
  m_statusBar->showMessage(tr("Ready"), 1000);

  m_editarea = new editarea(this);
  m_spritesbar = new spritesbar(this);
  m_colorsbar = new colorsbar(this);

  m_editarea->setForeGroundColor(m_colorsbar->getForegroundColor());
  m_editarea->setBackGroundColor(m_colorsbar->getBackgroundColor());

  auto img = m_spritesbar->getSelectedSprite();
  if (img) {
    m_editarea->setEditSprite(img);
  }

  connect(m_colorsbar, &colorsbar::foreGroundColorChanged, m_editarea,
          &editarea::setForeGroundColor);
  connect(m_colorsbar, &colorsbar::backGroundColorChanged, m_editarea,
          &editarea::setBackGroundColor);

  connect(m_spritesbar, &spritesbar::selectSpriteChanged, m_editarea,
          &editarea::setEditSprite);

  connect(m_editarea, &editarea::editSpriteChanged, m_spritesbar,
          &spritesbar::refreshDisplay);

  connect(m_editarea, &editarea::editImageChanged, m_spritesbar,
          &spritesbar::setCurrentImage);

  connect(m_editarea, &editarea::saveImageState, m_spritesbar,
          &spritesbar::saveCurSpriteState);

  connect(m_editarea, &editarea::restoreImageState, m_spritesbar,
          &spritesbar::restoreCurSpriteState);

  connect(m_editarea, &editarea::pickImageColor, m_colorsbar,
          &colorsbar::setForeGroundColor);

  createActions();
  createMenus();
  createToolBar();

  pencilModeAct->setChecked(true);
  m_editarea->setPencilMode();

  QHBoxLayout *hBox = new QHBoxLayout();
  hBox->addWidget(m_editarea);
  hBox->addWidget(m_spritesbar);

  QVBoxLayout *vBox = new QVBoxLayout();
  vBox->addLayout(hBox);
  vBox->addWidget(m_colorsbar);

  QWidget *w = new QWidget();
  w->setLayout(vBox);

  setCentralWidget(w);
  w->show();

  resize(640, 732);
}

MainWindow::~MainWindow() {}

void MainWindow::uncheckAllToolBarButtons() {
  selectModeAct->setChecked(false);
  pencilModeAct->setChecked(false);
  rectangleModeAct->setChecked(false);
  ellipseModeAct->setChecked(false);
  fillModeAct->setChecked(false);
}

void MainWindow::setSelectMode()
/*---------------------------------------------------------*\
 * Switch Editarea widget to <Box Select mode>
 * and update toolbar buttons
 *
 * 2024-04-21                       Raymond NGUYEN THANH
\*---------------------------------------------------------*/
{
  uncheckAllToolBarButtons();
  selectModeAct->setChecked(true);
  m_editarea->setSelectMode();
}

void MainWindow::setPencilMode()
/*---------------------------------------------------------*\
 * Switch Editarea widget to <Pencil draw mode>
 * and update toolbar buttons
 *
 * 2024-04-21                       Raymond NGUYEN THANH
\*---------------------------------------------------------*/
{
  uncheckAllToolBarButtons();
  pencilModeAct->setChecked(true)    // if (m_image){
      //      painter.drawImage(QPoint(200,200),*m_image);
      // }
;
  m_editarea->setPencilMode();
}

void MainWindow::setRectangleMode()
/*---------------------------------------------------------*\
 * Switch Editarea widget to <Rectangle draw mode>
 * and update toolbar buttons
 *
 * 2024-04-21                       Raymond NGUYEN THANH
\*---------------------------------------------------------*/
{
  uncheckAllToolBarButtons();
  rectangleModeAct->setChecked(true);
  m_editarea->setRectangleMode();
}

void MainWindow::setEllipseMode()
/*---------------------------------------------------------*\
 * Switch Editarea widget to <Ellipse draw mode>
 * and update toolbar buttons
 *
 * 2024-04-21                       Raymond NGUYEN THANH
\*---------------------------------------------------------*/
{
  uncheckAllToolBarButtons();
  ellipseModeAct->setChecked(true);
  m_editarea->setEllipseMode();
}

void MainWindow::setFillMode()
/*---------------------------------------------------------*\
 * Switch Editarea widget to <Flood fill mode>
 * and update toolbar buttons
 *
 * 2024-04-21                       Raymond NGUYEN THANH
\*---------------------------------------------------------*/
{
  uncheckAllToolBarButtons();
  fillModeAct->setChecked(true);
  m_editarea->setFloodFillMode();
}

void MainWindow::doPaste()
/*---------------------------------------------------------*\
 * Paste a copied image into the current sprite
 *
 * 2024-04-21                       Raymond NGUYEN THANH
\*---------------------------------------------------------*/
{
  setSelectMode();
  m_editarea->pasteSelectBox();
}

void MainWindow::newSprite() {
  //-----------------------------------------
  NewSpriteDlg dlg(this);
  auto result = dlg.exec();
  if (result == QDialog::Accepted) {
    m_spritesbar->newImage(dlg.m_width, dlg.m_height);
  }
}

void MainWindow::open() {
  //-----------------------------------------
  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
  if (!fileName.isEmpty())
    m_spritesbar->openImage(fileName);
}

void MainWindow::save() {
  //-----------------------------------------
  QString fileName = m_spritesbar->getSelectedSpriteFileName();
  if (fileName == "") {
    saveAs();
  } else {
    m_spritesbar->saveImage();
  }
}

void MainWindow::saveAs() {
  //-----------------------------------------
  QString fileFormat = "png";
  QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

  QString fileName =
      QFileDialog::getSaveFileName(this, tr("Save As"), initialPath,
                                   tr("%1 Files (*.%2);;All Files (*)")
                                       .arg(fileFormat.toUpper())
                                       .arg(fileFormat));
  if (!fileName.isEmpty()) {
    m_spritesbar->saveAsImage(fileName, fileFormat.toLatin1());
  }
}

void MainWindow::about() {
  //-----------------------------------------
  AboutDlg dlg(this);
  dlg.exec();
}

void MainWindow::undo() {
    //-----------------------------------------
    m_editarea->doUndo();
}

void MainWindow::createToolBar() {
  m_toolBar = new QToolBar("Test ToolBar");
  m_toolBar->setIconSize(QSize(24, 24));
  this->addToolBar(m_toolBar);

  m_toolBar->addAction(selectModeAct);

  m_toolBar->addAction(pencilModeAct);

  m_toolBar->addAction(rectangleModeAct);

  m_toolBar->addAction(ellipseModeAct);

  m_toolBar->addAction(fillModeAct);
}

void MainWindow::createActions() {
  // QIcon undoicon = QIcon::fromTheme("edit-undo");
  // QIcon undoicon = QIcon::fromTheme("edit-undo",
  // QIcon(":/res/SelectBoxIcon.png")); QIcon newicon =
  // QIcon::fromTheme("edit-undo", QIcon(":res/document-new-symbolic.svg"));
  // this->style()->standardIcon(QStyle::SP_TrashIcon);

  newAct = new QAction(
      QIcon::fromTheme("document-new", QIcon(":res/document-new-symbolic.svg")),
      tr("&New..."), this);
  newAct->setShortcuts(QKeySequence::New);
  connect(newAct, &QAction::triggered, this, &MainWindow::newSprite);

  openAct = new QAction(QIcon(":res/document-open-symbolic.svg"),
                        tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  connect(openAct, &QAction::triggered, this, &MainWindow::open);

  saveAct =
      new QAction(QIcon(":res/document-save-symbolic.svg"), tr("Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  connect(saveAct, &QAction::triggered, this, &MainWindow::save);

  saveAsAct = new QAction(QIcon(":res/document-save-as-symbolic.svg"),
                          tr("Save As..."), this);
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);

  printAct = new QAction(QIcon(":res/document-print-symbolic.svg"),
                         tr("&Print..."), this);
  // connect(printAct, &QAction::triggered, scribbleArea, &ScribbleArea::print);

  exitAct = new QAction(QIcon(":res/application-exit-symbolic.svg"),
                        tr("E&xit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  connect(exitAct, &QAction::triggered, this, &MainWindow::close);

  undoAct = new QAction(QIcon(":res/edit-undo-symbolic.svg"), tr("Undo"), this);
  undoAct->setShortcuts(QKeySequence::Undo);
  connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

  cutAct = new QAction(QIcon(":res/edit-cut-symbolic.svg"), tr("Cut"), this);
  cutAct->setShortcuts(QKeySequence::Cut);
  connect(cutAct, &QAction::triggered, m_editarea, &editarea::cutSelectBox);

  copyAct = new QAction(QIcon(":res/edit-copy-symbolic.svg"), tr("Copy"), this);
  copyAct->setShortcuts(QKeySequence::Copy);
  connect(copyAct, &QAction::triggered, m_editarea, &editarea::copySelectBox);

  pasteAct =
      new QAction(QIcon(":res/edit-paste-symbolic.svg"), tr("Paste"), this);
  pasteAct->setShortcuts(QKeySequence::Paste);
  connect(pasteAct, &QAction::triggered, this, &MainWindow::doPaste);

  aboutAct =
      new QAction(QIcon(":res/help-about-symbolic.svg"), tr("&About"), this);
  connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

  aboutQtAct = new QAction(tr("About &Qt"), this);
  connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);

  selectModeAct = new QAction(QIcon(":/res/SelectBoxIcon.png"), "Select", this);
  selectModeAct->setCheckable(true);
  connect(selectModeAct, &QAction::triggered, this, &MainWindow::setSelectMode);

  pencilModeAct = new QAction(QIcon(":/res/PencilIcon.png"), "Pencil", this);
  pencilModeAct->setCheckable(true);
  connect(pencilModeAct, &QAction::triggered, this, &MainWindow::setPencilMode);

  rectangleModeAct =
      new QAction(QIcon(":/res/RectangleIcon.png"), "Pencil", this);
  rectangleModeAct->setCheckable(true);
  connect(rectangleModeAct, &QAction::triggered, this,
          &MainWindow::setRectangleMode);

  ellipseModeAct = new QAction(QIcon(":/res/EllipseIcon.png"), "Pencil", this);
  ellipseModeAct->setCheckable(true);
  connect(ellipseModeAct, &QAction::triggered, this,
          &MainWindow::setEllipseMode);

  fillModeAct = new QAction(QIcon(":/res/FloodFillIcon.png"), "Pencil", this);
  fillModeAct->setCheckable(true);
  connect(fillModeAct, &QAction::triggered, this, &MainWindow::setFillMode);
}

void MainWindow::createMenus() {
  fileMenu = new QMenu(tr("&File"), this);
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addSeparator();
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addAction(printAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  editMenu = new QMenu(tr("&Edit"), this);
  editMenu->addAction(undoAct);
  editMenu->addSeparator();
  editMenu->addAction(cutAct);
  editMenu->addSeparator();
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);

  helpMenu = new QMenu(tr("&Help"), this);
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);

  menuBar()->addMenu(fileMenu);
  menuBar()->addMenu(editMenu);
  menuBar()->addMenu(helpMenu);
}
