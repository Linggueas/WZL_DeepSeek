#ifndef SQLMGR_H
#define SQLMGR_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>
class SQLMgr : public QObject
{
    Q_OBJECT
public:
    explicit SQLMgr(QObject *parent = nullptr);
    void save_message(const std::set<int> &shift_id, const QStringList &titles,const QVector<QJsonArray> &history);
    //static void save_message_async(const QStringList &titles, const QVector<QJsonArray> &history);
    void insert_message();
    void init_message();
    void delete_message(int id);
    QString select_one_json(int id);
private:
    QSqlDatabase sql_db;
signals:
    void sig_init(const QStringList& title);
};

#endif // SQLMGR_H
