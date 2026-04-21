#include "filedialog.h"
#include "ui_filedialog.h"

FileDialog::FileDialog(QWidget *parent,const QString &file_name)
    : QWidget(parent)
    , ui(new Ui::FileDialog)
{
    ui->setupUi(this);
    setStyleSheet("background-color: white;");
    QPixmap pixmap(":/新前缀/file.png");
    pixmap = pixmap.scaled(ui->icon_label->size());
    ui->icon_label->setPixmap(pixmap);
    ui->name_label->setText(file_name);
}

FileDialog::~FileDialog()
{
    delete ui;
}

QString FileDialog::getFileName() const
{
    return ui->name_label->text();
}

void FileDialog::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked();
}
