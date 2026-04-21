#include "ui_filelistdialog.h"
#include "filedialog.h"
#include "filelistdialog.h"
#include "deletedialog.h"
FileListDialog::FileListDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FileListDialog)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    ui->tableWidget->setRowCount(1);
    
    // 设置QTableWidget的样式和属性
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setStyleSheet("QTableWidget { border: none; background-color: white; } QTableWidget::item { border: none; }");
    
    // 设置行高和列宽
    ui->tableWidget->setRowHeight(0, 119); // 与FileDialog高度匹配

}

FileListDialog::~FileListDialog()
{
    delete ui;

}

void FileListDialog::add_file(QString file_name)
{
    int col = ui->tableWidget->columnCount();
    ui->tableWidget->setColumnCount(col + 1);
    FileDialog *file_dlg = new FileDialog(this,file_name);
    ui->tableWidget->setCellWidget(0,ui->tableWidget->columnCount()-1,file_dlg);
    connect(file_dlg, &FileDialog::clicked, this, [=]() {
        ui->tableWidget->setCurrentCell(0, col);
        emit ui->tableWidget->cellClicked(0, col);
    });

}

void FileListDialog::clear_all()
{
    ui->tableWidget->clear();
}

bool FileListDialog::empty()
{
    if(ui->tableWidget->columnCount() == 0)
    {
        return true;
    }
    return false;
}



void FileListDialog::on_tableWidget_cellClicked(int row, int column)
{
    DeleteDialog*dl_dlg = new DeleteDialog(this);
    dl_dlg->show();
    dl_dlg->setAttribute(Qt::WA_DeleteOnClose);
    connect(dl_dlg,&DeleteDialog::sig_yes_delete,this,[this,dl_dlg,row,column](){
        FileDialog *f_dlg = qobject_cast<FileDialog*>(ui->tableWidget->cellWidget(row,column));
        ui->tableWidget->removeCellWidget(row,column);
        dl_dlg->close();
        emit slot_delete(f_dlg->getFileName());
        f_dlg->deleteLater();
    });
    connect(dl_dlg,&DeleteDialog::sig_no_delete,this,[this,dl_dlg]{
        dl_dlg->close();
    });
}

