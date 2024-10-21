#include "newspritedialog.h"
#include "ui_newspritedialog.h"

NewSpriteDialog::NewSpriteDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewSpriteDialog)
{
    ui->setupUi(this);
}

NewSpriteDialog::~NewSpriteDialog()
{
    delete ui;
}


void NewSpriteDialog::on_buttonBox_accepted()
{
    m_width = ui->spinBoxWidth->value();
    m_height = ui->spinBoxHeight->value();
}

void NewSpriteDialog::on_buttonBox_rejected()
{

}

