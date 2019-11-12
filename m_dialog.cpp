#include "m_dialog.h"
#include "ui_m_dialog.h"
#include <QDebug>
#include <QPushButton>

M_Dialog::M_Dialog(QWidget *parent,bool buttonshow,bool inputshow) :
    QDialog(parent),
    ui(new Ui::M_Dialog)
{
    ui->setupUi(this);
    if(!buttonshow)
    {
        ui->buttonBox->hide();
    }
    else
    {
        QPushButton *cal = ui->buttonBox->button(((QDialogButtonBox::Cancel)));
        cal->setText(tr("黑白"));
        QPushButton *button = ui->buttonBox->button((QDialogButtonBox::Ok));
        button->setText(tr("彩图"));
    }
    if(!inputshow)
    {
        ui->thresholdEdit->hide();
    }
}

M_Dialog::~M_Dialog()
{
    delete ui;
}

int M_Dialog::getValue()
{
    return qBound(0,ui->thresholdEdit->text().toInt(),255);
}

void M_Dialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if((QPushButton*)button == ui->buttonBox->button((QDialogButtonBox::Cancel)))
    {
        this->reject();
    }
    if((QPushButton*)button == ui->buttonBox->button((QDialogButtonBox::Ok)))
    {
        this->accept();
    }
}
