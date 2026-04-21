#ifndef FILELISTDIALOG_H
#define FILELISTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTableWidgetItem>
namespace Ui {
class FileListDialog;
}

class FileListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileListDialog(QWidget *parent = nullptr);
    ~FileListDialog();
    void add_file(QString file_name);
    void clear_all();
    bool empty();
private slots:
    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::FileListDialog *ui;
    QStandardItemModel* model;
signals:
    void slot_delete(QString file_name);

};

#endif // FILELISTDIALOG_H
