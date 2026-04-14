#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "sqlmgr.h"
#include <QMainWindow>
#include <QStringListModel>
#include <QVector>
#include <QJsonArray>
#include <QJsonObject>
#include <httpmgr.h>
#include <QSettings>
QT_BEGIN_NAMESPACE
extern QString key;
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

    void slot_read_data(QString data);
    void slot_read_reasoning(QString reasoning);
    void slot_streamFinished();
    void on_listView_2_clicked(const QModelIndex &index);

    void slot_delete();

    void slot_close_widget();

    void slot_init(const QStringList& title,const QStringList& message);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QStringListModel*left_model;
    QVector<QStringListModel*>right_model;
    QVector<QJsonArray>history;
    int now_index;
    bool deep_think;
    bool have_data;
    bool have_reasoning;
    HttpMgr*http_mgr;
    void dispose_json();
    void closeEvent(QCloseEvent *event)override;
    QString all_data;
    QString all_reasoning;
    SQLMgr *sql_mgr;
signals:
    void send_data(QJsonObject json);

};
#endif // MAINWINDOW_H
