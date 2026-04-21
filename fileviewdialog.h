#ifndef FILEVIEWDIALOG_H
#define FILEVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class FileViewDialog;
}

class FileViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileViewDialog(QWidget *parent = nullptr);
    ~FileViewDialog();
    void set_file_view(QByteArray data);
private:
    Ui::FileViewDialog *ui;
};

#endif // FILEVIEWDIALOG_H
