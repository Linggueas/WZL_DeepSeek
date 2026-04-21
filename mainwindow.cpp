#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QFileDialog>
#include <QStandardItemModel>
#include <deletedialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),now_index(-1)
    , ui(new Ui::MainWindow),deep_think(false),
    all_data("回答内容\n"),have_data(false),
    all_reasoning("思考内容\n"),have_reasoning(false),file_button_bool(false)
{
    ui->setupUi(this);
    left_model = new QStringListModel(this);
    http_mgr = new HttpMgr(this);
    ui->listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setModel(left_model);

    sql_mgr = new SQLMgr(this);
    fl_dlg = new FileListDialog(this);
    fl_dlg->setWindowFlag(Qt::FramelessWindowHint);

    file_button = new QPushButton(this);
    file_button->resize(30, 30);
    file_button->move(970,470);
    file_button->setIcon(QIcon(":/新前缀/file.png"));
    file_button->setIconSize(file_button->size());
    file_button->show();

    connect(this,&MainWindow::send_data,http_mgr,&HttpMgr::send_request);
    connect(http_mgr,&HttpMgr::newToken,this,&MainWindow::slot_read_data);
    connect(http_mgr,&HttpMgr::streamFinished,this,&MainWindow::slot_streamFinished);
    connect(http_mgr,&HttpMgr::newReasoningToken,this,&MainWindow::slot_read_reasoning);
    connect(ui->listView,&my_list_view::sign_delete,this,&MainWindow::slot_delete);
    connect(sql_mgr,&SQLMgr::sig_init,this,&MainWindow::slot_init);
    connect(file_button,&QPushButton::clicked,this,&MainWindow::on_file_pushbutton_clicked);
    connect(fl_dlg,&FileListDialog::slot_delete,this,[this](QString file_name){
        send_file.erase(file_name);
    });

    QListView* list_view = qobject_cast<QListView*>(ui->listView);
    if(list_view){
        list_view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    }

    QListView* list_view_2 = qobject_cast<QListView*>(ui->listView_2);
    if(list_view_2){
        list_view_2->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    }

    sql_mgr->init_message();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_send_pushButton_clicked()
{
    QString message = ui->textEdit->toPlainText();
    if(message == "")
    {
        QMessageBox::warning(this,"警告","请输入文本");
        return;
    }
    if(now_index == -1)
    {
        now_index = 0;
        QJsonArray json_array;
        history.push_back(json_array);
        left_model->insertRow(left_model->rowCount());
        left_model->setData(left_model->index(left_model->rowCount()-1),"");
        QStringListModel*sl_model = new QStringListModel(this);
        ui->listView_2->setModel(sl_model);
        right_model.push_back(sl_model);
        sql_mgr->insert_message();
    }
    if(right_model[now_index]->rowCount() == 0)
    {
        left_model->setData(left_model->index(now_index),message.left(30));
    }
    QJsonObject json_obj;
    json_obj["role"] = "user";
    json_obj["content"] = message;
    history[now_index].append(json_obj);

    right_model[now_index]->insertRow(right_model[now_index]->rowCount());
    right_model[now_index]->setData(right_model[now_index]->index(right_model[now_index]->rowCount()-1),message);

    ui->textEdit->clear();

    dispose_json();
}


void MainWindow::on_deep_think_pushButton_clicked()
{
    if(deep_think)
    {
        deep_think = false;
        ui->deep_think_pushButton->setStyleSheet("QPushButton{"
"background-color:#e0e0e0;"
"color:#1a1a1a;"
"border:none;"
"border-radius:6px;"
"padding:8px 16px;"
"}"
);
    }else
    {
        deep_think = true;
        ui->deep_think_pushButton->setStyleSheet("QPushButton{"
"background-color: rgb(85, 255, 255);"
"color:#1a1a1a;"
"border:none;"
"border-radius:6px;"
"padding:8px 16px;"
"}"
                                                 );
    }
}
//处理要发送的json文件
void MainWindow::dispose_json()
{
    QJsonObject json_obj;
    json_obj["model"] = "deepseek-chat";
    json_obj["messages"] = history[now_index];
    json_obj["stream"] = true;
    json_obj["temperature"] = 0.7;
    if(deep_think)
    {
        json_obj["model"] = "deepseek-reasoner";
    }
    
    // 传递文件给HttpMgr
    http_mgr->set_files(send_file);
    
    emit send_data(json_obj);
    ui->send_pushButton->setEnabled(false);
    ui->send_pushButton->setStyleSheet("background-color: rgb(186, 186, 186);");
    ui->listView->setEnabled(false);
    ui->new_dlg_pushButton->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent*event)
{
    Q_UNUSED(event)
    //qDebug()<<"fuck";
    sql_mgr->save_message(left_model,history);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    file_button->move(this->width()-30,this->height()-200);
    QWidget::resizeEvent(event);
}


void MainWindow::on_new_dlg_pushButton_clicked()
{
    left_model->insertRow(left_model->rowCount());
    left_model->setData(left_model->index(left_model->rowCount()-1),"");
    QJsonArray json_array;
    history.push_back(json_array);
    QStringListModel *stringmodel = new QStringListModel(this);
    right_model.push_back(stringmodel);
    ui->listView_2->setModel(right_model[left_model->rowCount()-1]);
    now_index = left_model->rowCount()-1;
    sql_mgr->insert_message();
}


void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    now_index = index.row();
    ui->listView_2->setModel(right_model[now_index]);
}

void MainWindow::on_file_pushbutton_clicked()
{
    if(!file_button_bool)
    {
        QPoint btnPos = file_button->mapToGlobal(QPoint(0-fl_dlg->width(), 0-fl_dlg->height()));
        fl_dlg->move(btnPos);
        fl_dlg->show();
        file_button_bool = true;
    }else{
        fl_dlg->hide();
        file_button_bool = false;
    }

}

void MainWindow::slot_read_data(QString data)
{
    all_data.append(data);
    if(!have_data)
    {
        have_data = true;
        right_model[now_index]->insertRow(right_model[now_index]->rowCount());
        right_model[now_index]->setData(right_model[now_index]->index(right_model[now_index]->rowCount()-1),data);
        return;
    }
    right_model[now_index]->setData(right_model[now_index]->index(right_model[now_index]->rowCount()-1),all_data);

}

void MainWindow::slot_read_reasoning(QString reasoning)
{
    all_reasoning.append(reasoning);
    if(!have_reasoning)
    {
        have_reasoning = true;
        right_model[now_index]->insertRow(right_model[now_index]->rowCount());
        right_model[now_index]->setData(right_model[now_index]->index(right_model[now_index]->rowCount()-1),reasoning);
    }
    right_model[now_index]->setData(right_model[now_index]->index(right_model[now_index]->rowCount()-1),all_reasoning);

}

void MainWindow::slot_streamFinished()
{
    have_data = false;
    have_reasoning = false;
    QJsonObject json_obj;
    json_obj["role"] = "assistant";
    json_obj["content"] = all_data;
    history[now_index].append(json_obj);
    all_data = "回答内容\n";
    all_reasoning = "思考内容\n";
    ui->send_pushButton->setEnabled(true);
    ui->send_pushButton->setStyleSheet("QPushButton{"
"background-color:#007bff;"
"color:#ffffff;"
"border:none;"
"border-radius:6px;"
"padding:8px 24px;"
"}"
"QPushButton:hover{"
"background-color:#0056b3;"
"}"
"QPushButton:pressed{"
"background-color:#004494;"
"}");
    ui->listView->setEnabled(true);
    ui->new_dlg_pushButton->setEnabled(true);
    send_file.clear();
    fl_dlg->clear_all();
}

void MainWindow::on_listView_2_clicked(const QModelIndex &index)
{
    QString save_file = QFileDialog::getSaveFileName( this,
                                                     tr("deepseek回答.txt"),
                                                     QDir::homePath(),
                                                     tr("文本文件 (*.txt);;所有文件 (*.*)") // 文件类型过滤器
                                                     );

    QFile file(save_file);
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QString data =  ui->listView_2->model()->data(index).toString();
    file.write(data.toUtf8());
    file.close();

}

void MainWindow::slot_delete()
{
    if(now_index == -1)
    {
        return;
    }
    //ui->listView->setCurrentIndex();
    DeleteDialog *dl_dlg = new DeleteDialog(this);
    dl_dlg->setAttribute(Qt::WA_DeleteOnClose);
    dl_dlg->show();
    connect(dl_dlg,&DeleteDialog::sig_yes_delete,this,[this,dl_dlg](){
        dl_dlg->close();
        left_model->removeRows(now_index,1);
        history.removeOne(history[now_index]);
        right_model[now_index]->deleteLater();
        right_model.removeOne(right_model[now_index]);
        ui->listView_2->setModel(nullptr);
        sql_mgr->delete_message(now_index);
        now_index = -1;

    });
    connect(dl_dlg,&DeleteDialog::sig_no_delete,this,[this,dl_dlg](){
        dl_dlg->close();
    });

}

void MainWindow::slot_close_widget()
{

}

void MainWindow::slot_init(const QStringList &title, const QStringList &message)
{
    for(int i = 0;i<title.size();i++)
    {
        left_model->insertRow(left_model->rowCount());
        left_model->setData(left_model->index(left_model->rowCount()-1),title[i]);
        QStringListModel* stringlistmodel = new QStringListModel(this);
        QJsonDocument json_doc = QJsonDocument::fromJson(message[i].toUtf8());
        QJsonArray json_arr = json_doc.array();
        history.push_back(json_arr);
        for(int j = 0;j<json_arr.size();j++)
        {
            QJsonObject json_obj = json_arr[j].toObject();
            if(json_obj["role"].toString() == "user")
            {
                stringlistmodel->insertRow(stringlistmodel->rowCount());
                stringlistmodel->setData(stringlistmodel->index(stringlistmodel->rowCount()-1),
                                         json_obj["content"].toString());
            }
            if(json_obj["role"].toString() == "assistant")
            {
                QString data;
                data.append(json_obj["content"].toString());
                stringlistmodel->insertRow(stringlistmodel->rowCount());
                stringlistmodel->setData(stringlistmodel->index(stringlistmodel->rowCount()-1),
                                         data);

            }
        }
        right_model.push_back(stringlistmodel);
    }
    ui->listView->setEnabled(true);
    ui->listView_2->setEnabled(true);
}




void MainWindow::on_add_file_pushButton_clicked()
{
    QString file_dir = QFileDialog::getOpenFileName(
        this,  // 确保this指针有效
        tr("打开文件"),
        "D:/",
        tr("所有文件 (*)")
    );
    QFile file(file_dir);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"错误","文件打开失败");
        return;
    }
    QByteArray file_byte = file.readAll();
    QString file_name = (file_dir.split("/"))[file_dir.split("/").size()-1];

    send_file.insert(std::make_pair(file_name,file_byte));
    fl_dlg->add_file(file_name);
}

