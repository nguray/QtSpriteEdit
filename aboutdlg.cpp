#include "aboutdlg.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

AboutDlg::AboutDlg(QWidget *parent): QDialog{parent}
{
    setMinimumWidth(200);
    setMinimumHeight(128);

    QLabel *l1 = new QLabel("Qt Sprite Editor",this);
    l1->setFont(QFont( "Arial", 12, QFont::Bold));
    QLabel *l2 = new QLabel("version 0.1",this);
    QLabel *l3 = new QLabel("Raymond NGUYEN THANH",this);
    QVBoxLayout * vBoxLayout = new QVBoxLayout(this);
    QHBoxLayout * hBoxLayout = new QHBoxLayout(this);

    QPushButton *okBtn = new QPushButton("Ok",this);

    vBoxLayout->addWidget(l1,0,Qt::AlignCenter);
    vBoxLayout->addWidget(l2,0,Qt::AlignCenter);
    vBoxLayout->addWidget(l3,0,Qt::AlignCenter);

    hBoxLayout->addStretch();
    hBoxLayout->addWidget(okBtn);
    hBoxLayout->addStretch();

    vBoxLayout->addStretch();
    vBoxLayout->addLayout(hBoxLayout);

    connect(okBtn, &QPushButton::pressed, this, &AboutDlg::okBtnClick);

    setLayout(vBoxLayout);
    setWindowTitle(tr("About"));


}

void AboutDlg::okBtnClick()
{

    accept();
}
