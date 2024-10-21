#ifndef NEWSPRITEDIALOG_H
#define NEWSPRITEDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class NewSpriteDialog;
}

class NewSpriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewSpriteDialog(QWidget *parent = nullptr);
    ~NewSpriteDialog();

    int m_width;
    int m_height;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();

private:
    Ui::NewSpriteDialog *ui;
};

#endif // NEWSPRITEDIALOG_H
