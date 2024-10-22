#include "newsprite2dialog.h"
#include "ui_newsprite2dialog.h"

NewSprite2Dialog::NewSprite2Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewSprite2Dialog)
{
    ui->setupUi(this);

    //connect(ui->OkBtn, &QPushButton::pressed, this, &NewSprite2Dialog::on_ok_click);
    //connect(ui->CancelBtn, &QPushButton::pressed, this, &NewSprite2Dialog::on_cancel_click);

}

NewSprite2Dialog::~NewSprite2Dialog()
{
    delete ui;
}

void NewSprite2Dialog::on_CancelBtn_clicked()
{
    reject();

}

void NewSprite2Dialog::on_OkBtn_clicked()
{
    m_width = ui->spinBoxWidth->value();
    m_height = ui->spinBoxHeight->value();
    if (m_width && m_height){
        accept();
    }

}

