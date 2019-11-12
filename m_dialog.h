#ifndef M_DIALOG_H
#define M_DIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class M_Dialog;
}

class M_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit M_Dialog(QWidget *parent = nullptr);
    ~M_Dialog();
    int getValue();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::M_Dialog *ui;
};

#endif // M_DIALOG_H
