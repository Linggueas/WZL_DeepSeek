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
    init_table();

}

void SQLMgr::save_message(const std::set<int> &shift_id, const QStringList &titles,const QVector<QJsonArray> &history)
{
    for(const auto &id:shift_id)
    {
        QSqlQuery sql_que;
        sql_que.prepare("update message_table "
                        "set title = :title , message = :message "
                        "where id = :id");
        sql_que.bindValue(":title",titles[id]);
        sql_que.bindValue(":id",id);
        sql_que.bindValue(":message",QString::fromUtf8(QJsonDocument(history[id]).toJson()));
        qDebug()<<QString::fromUtf8(QJsonDocument(history[id]).toJson());
        sql_que.exec();
    }
}

void SQLMgr::init_table()
{
    QSqlQuery sql_que;
    sql_que.prepare("create table IF NOT EXISTS message_table("
                    "id integer,"
                    "title varchar(99),"
                    "message json"
                    ")");
    sql_que.exec();
}

/*void SQLMgr::save_message_async(const QVector &id_vec, const QVector<QJsonArray> &history)
{

}*/

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
    sql_que.prepare("select title "
                    "from message_table");
    sql_que.exec();
    QStringList title;
    while(sql_que.next())
    {
        if(sql_que.value(0).isNull())
        {
            return;
        }
        title.push_back(sql_que.value(0).toString());
    }
    emit sig_init(title);
}

void SQLMgr::delete_message(int id)
{
    sql_db.transaction();
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
    sql_db.commit();
}

QString SQLMgr::select_one_json(int id)
{
    QSqlQuery sql_que;
    sql_que.prepare("select message from message_table where id = :id");
    sql_que.bindValue(":id",id);
    sql_que.exec();
    sql_que.next();
    return sql_que.value(0).toString();
}


