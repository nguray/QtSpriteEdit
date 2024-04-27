#ifndef NEWSPRITEDLG_H
#define NEWSPRITEDLG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>

class NewSpriteDlg : public QDialog
{
    Q_OBJECT
public:
    NewSpriteDlg(QWidget *parent = nullptr);
    QPushButton *m_okBtn;
    QPushButton *m_cancelBtn;
    QLineEdit   *m_widthEdit;
    QLineEdit   *m_heightEdit;

    int         m_width;
    int         m_height;

private slots:
    void    okBtnClick();
    void    cancelBtnClick();

};

#endif // NEWSPRITEDLG_H
