#include "deletedialog.h"
#include "ui_deletedialog.h"

DeleteDialog::DeleteDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteDialog)
{
    ui->setupUi(this);
}

DeleteDialog::~DeleteDialog()
{
    delete ui;
}
//yes
void DeleteDialog::on_pushButton_clicked()
{
    emit sig_yes_delete();
}

//no
void DeleteDialog::on_pushButton_2_clicked()
{
    emit sig_no_delete();
}

