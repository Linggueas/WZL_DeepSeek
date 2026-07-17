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
#include <QTimer>
#include <filelistdialog.h>
#include <markdowndelegate.h>
#include <map>
#include <QListView>
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
    void flush_batch();
    void on_listView_2_clicked(const QModelIndex &index);

    void slot_delete();

    void slot_close_widget();

    void slot_show_raw_text(const QPoint &pos);

    void on_toggle_question_clicked();

    void on_question_listView_clicked(const QModelIndex &index);

    void slot_init(const QStringList& title);


    void on_add_file_pushButton_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QStringListModel*left_model;
    QVector<QStringListModel*>right_model;
    QVector<QStringListModel*>question_model;
    QVector<QJsonArray>history;
    std::map<QString,QByteArray> send_file;
    std::set<int>shift_id;

    int now_index;
    bool deep_think;
    bool have_data;
    bool have_reasoning;
    HttpMgr*http_mgr;
    void dispose_json();

    void closeEvent(QCloseEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;
    void moveEvent(QMoveEvent *event)override;
    bool eventFilter(QObject *obj, QEvent *event)override;


    QString api_key;
    QString all_data;
    QString all_reasoning;
    QString m_pending_data;
    QString m_pending_reasoning;
    QTimer *m_batch_timer;
    bool m_pending_new_data;
    bool m_pending_new_reasoning;
    QString model;
    SQLMgr *sql_mgr;
    QPushButton*file_button;
    FileListDialog *fl_dlg;
    MarkdownDelegate *m_md_delegate;

    QListView *question_listView;
    QPushButton *toggle_question_btn;
    bool question_visible;

    bool file_button_bool;
signals:
    void send_data(QJsonObject json);
    void set_api_key(QString key);

};
#endif // MAINWINDOW_H
