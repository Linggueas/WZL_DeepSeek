#include "fileviewdialog.h"
#include "ui_fileviewdialog.h"

FileViewDialog::FileViewDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FileViewDialog)
{
    ui->setupUi(this);
}

FileViewDialog::~FileViewDialog()
{
    delete ui;
}

void FileViewDialog::set_file_view(QByteArray data)
{

}
