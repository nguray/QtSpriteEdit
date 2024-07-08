#ifndef NEWSPRITEDLG_H
#define NEWSPRITEDLG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>


class NewSpriteDlg : public QDialog {
  Q_OBJECT
public:
  NewSpriteDlg(QWidget *parent = nullptr);
  QPushButton   *m_okBtn;
  QPushButton   *m_cancelBtn;
  QSpinBox      *m_widthEdit;
  QSpinBox      *m_heightEdit;

  int m_width;
  int m_height;

private slots:
  void okBtnClick();
  void cancelBtnClick();
};

#endif // NEWSPRITEDLG_H
