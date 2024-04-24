#include "newspritedlg.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFontMetrics>

NewSpriteDlg::NewSpriteDlg(QWidget *parent): QDialog{parent}
{
    //setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumWidth(200);
    setMinimumHeight(128);

    QFrame *sizeFrame = new QFrame(this);

    QVBoxLayout * vBoxLayout = new QVBoxLayout(this);
    QVBoxLayout * vBoxLayout1 = new QVBoxLayout(this);

    QHBoxLayout * hBoxLayout1 = new QHBoxLayout(this);
    QLabel * widthLabel = new QLabel(this);
    widthLabel->setText("Width");
    widthLabel->setMinimumWidth(50);
    m_widthEdit = new QLineEdit("32");
    m_widthEdit->setAlignment(Qt::AlignRight);
    m_widthEdit->setMaxLength(4);
    m_widthEdit->setMaximumWidth(50);
    hBoxLayout1->addStretch();
    hBoxLayout1->addWidget(widthLabel,0,Qt::AlignLeft);
    hBoxLayout1->addWidget(m_widthEdit,0,Qt::AlignRight);
    hBoxLayout1->addStretch();

    QHBoxLayout * hBoxLayout2 = new QHBoxLayout(this);
    QLabel *heightLabel = new QLabel(this);
    heightLabel->setText("Height");
    heightLabel->setMinimumWidth(50);
    m_heightEdit = new QLineEdit("32");
    m_heightEdit->setAlignment(Qt::AlignRight);
    m_heightEdit->setMaxLength(4);
    m_heightEdit->setMaximumWidth(50);
    hBoxLayout2->addStretch();
    hBoxLayout2->addWidget(heightLabel,0,Qt::AlignLeft);
    hBoxLayout2->addWidget(m_heightEdit,0,Qt::AlignRight);
    hBoxLayout2->addStretch();

    QFontMetrics fm = fontMetrics();
    QRect rect1 = fm.boundingRect("Width");
    QRect rect2 = fm.boundingRect("Height");


    vBoxLayout1->addLayout(hBoxLayout1);
    vBoxLayout1->addLayout(hBoxLayout2);
    vBoxLayout1->addStretch();

    sizeFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    sizeFrame->setFrameShadow(QFrame::Sunken);
    //sizeFrame->setLineWidth(3);
    sizeFrame->setLayout(vBoxLayout1);

    QHBoxLayout * hBoxLayout3 = new QHBoxLayout(this);
    m_okBtn = new QPushButton("Ok",this);
    m_cancelBtn = new QPushButton("Cancel",this);
    hBoxLayout3->addStretch();
    hBoxLayout3->addWidget(m_okBtn,0,Qt::AlignLeft);
    hBoxLayout3->addWidget(m_cancelBtn,0,Qt::AlignRight);
    hBoxLayout3->addStretch();

    vBoxLayout->addWidget(sizeFrame);
    vBoxLayout->addLayout(hBoxLayout3);

    connect(m_okBtn, &QPushButton::pressed, this, &NewSpriteDlg::okBtnClick);
    connect(m_cancelBtn, &QPushButton::pressed, this, &NewSpriteDlg::cancelBtnClick);

    setLayout(vBoxLayout);
    setWindowTitle("Image sizes");

}

void NewSpriteDlg::okBtnClick()
{
    //----------------------------------------------
    QString strWith   = m_widthEdit->text();
    QString strHeight = m_heightEdit->text();
    m_width  = strWith.toInt();
    m_height = strHeight.toInt();
    if ((m_width!=0)&&(m_height!=0)){
        accept();
    }
}

void NewSpriteDlg::cancelBtnClick()
{
    //----------------------------------------------
    m_width  = 0;
    m_height = 0;
    reject();
}
