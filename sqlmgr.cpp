#include "sqlmgr.h"
#include "QSqlQuery"
#include "QJsonArray"
#include "QJsonDocument"
#include "QJsonObject"
SQLMgr::SQLMgr(QObject *parent)
    : QObject{parent}
{
    sql_db = QSqlDatabase::addDatabase("QSQLITE");
    sql_db.setDatabaseName("my.sqlite");
    if(!sql_db.open())
    {
        qDebug()<<"数据库连接失败";
        return;
    }

}

void SQLMgr::save_message(QStringListModel *left_model, QVector<QJsonArray> history)
{

    std::vector<int> id_vec;
    QSqlQuery sql_que;
    sql_que.prepare("select id from message_table");
    sql_que.exec();
    while(sql_que.next())
    {
        id_vec.push_back(sql_que.value(0).toInt());
    }

    for(int index = 0;index< history.size();index++)
    {
        QSqlQuery sql_que;
        sql_que.prepare("update message_table "
                        "set title = :title , message = :message "
                        "where id = :id");
        sql_que.bindValue(":title",left_model->index(index).data().toString());
        sql_que.bindValue(":id",id_vec[index]);
        sql_que.bindValue(":message",QString::fromUtf8(QJsonDocument(history[index]).toJson()));
        sql_que.exec();
    }
}

void SQLMgr::insert_message()
{
    QSqlQuery sql_que;
    sql_que.prepare("insert into message_table "
                    "values(:id,'','')");
    QSqlQuery sql_que1;
    sql_que1.prepare("select MAX(id) from message_table");
    sql_que1.exec();


    if (sql_que1.next()) {
        if(sql_que1.value(0).isNull())
        {
            sql_que.bindValue(":id",0);
        }else{
            sql_que.bindValue(":id",sql_que1.value(0).toInt()+1);
        }
    }
    sql_que.exec();
}

void SQLMgr::init_message()
{
    QSqlQuery sql_que;
    sql_que.prepare("select title,message "
                    "from message_table");
    sql_que.exec();
    QStringList title;
    QStringList message;
    while(sql_que.next())
    {
        if(sql_que.value(0).isNull())
        {
            return;
        }
        title.push_back(sql_que.value(0).toString());
        message.push_back(sql_que.value(1).toString());
    }
    emit sig_init(title,message);

}

void SQLMgr::delete_message(int id)
{
    QSqlQuery sql_que;
    sql_que.prepare("delete from message_table "
                    "where id = :id");
    sql_que.bindValue(":id",id);
    sql_que.exec();

    QSqlQuery sql_que1;
    sql_que1.prepare("update message_table "
                     "set id = id - 1 "
                     "where id > :id");
    sql_que1.bindValue(":id",id);
    sql_que1.exec();
}


