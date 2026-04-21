#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "sqlmgr.h"
#include <QMainWindow>
#include <QStringListModel>
#include <QVector>
#include <QJsonArray>
#include <QJsonObject>
#include <httpmgr.h>
#include <QPushButton>
#include <filelistdialog.h>
#include <map>
QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_send_pushButton_clicked();

    void on_deep_think_pushButton_clicked();

    void on_new_dlg_pushButton_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_file_pushbutton_clicked();

    void slot_read_data(QString data);
    void slot_read_reasoning(QString reasoning);
    void slot_streamFinished();
    void on_listView_2_clicked(const QModelIndex &index);

    void slot_delete();

    void slot_close_widget();

    void slot_init(const QStringList& title,const QStringList& message);


    void on_add_file_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QStringListModel*left_model;
    QVector<QStringListModel*>right_model;
    QVector<QJsonArray>history;
    std::map<QString,QByteArray> send_file;

    int now_index;
    bool deep_think;
    bool have_data;
    bool have_reasoning;
    HttpMgr*http_mgr;
    void dispose_json();

    void closeEvent(QCloseEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;
    QString all_data;
    QString all_reasoning;
    SQLMgr *sql_mgr;
    QPushButton*file_button;
    FileListDialog *fl_dlg;

    bool file_button_bool;
signals:
    void send_data(QJsonObject json);

};
#endif // MAINWINDOW_H
