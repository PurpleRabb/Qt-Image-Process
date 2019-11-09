#include "m_dialog.h"
#include "ui_m_dialog.h"
#include <QDebug>

M_Dialog::M_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::M_Dialog)
{
    ui->setupUi(this);
}

M_Dialog::~M_Dialog()
{
    delete ui;
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
