#ifndef NEWSPRITE2DIALOG_H
#define NEWSPRITE2DIALOG_H

#include <QDialog>

namespace Ui {
class NewSprite2Dialog;
}

class NewSprite2Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewSprite2Dialog(QWidget *parent = nullptr);
    ~NewSprite2Dialog();

    int m_width;
    int m_height;

private slots:

    void on_CancelBtn_clicked();

    void on_OkBtn_clicked();


private:
    Ui::NewSprite2Dialog *ui;
};

#endif // NEWSPRITE2DIALOG_H
