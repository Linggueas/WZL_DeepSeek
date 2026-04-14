#ifndef SQLMGR_H
#define SQLMGR_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringListModel>
class SQLMgr : public QObject
{
    Q_OBJECT
public:
    explicit SQLMgr(QObject *parent = nullptr);
    void save_message(QStringListModel*left_model,
                      QVector<QJsonArray>history);
    void insert_message();
    void init_message();
    void delete_message(int id);
private:
    QSqlDatabase sql_db;
signals:
    void sig_init(const QStringList& title,const QStringList& message);
};

#endif // SQLMGR_H
