#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>

class AboutDlg : public QDialog
{
    Q_OBJECT
public:
    AboutDlg(QWidget *parent = nullptr);

private slots:

    void    okBtnClick();

};

#endif // ABOUTDLG_H
