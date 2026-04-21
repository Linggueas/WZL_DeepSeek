#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include<QJsonArray>
#include <map>
#include <QByteArray>
class HttpMgr : public QObject
{
    Q_OBJECT
public:
    explicit HttpMgr(QObject *parent = nullptr);
    void set_files(const std::map<QString, QByteArray> &files);
private:
    QNetworkAccessManager*net_mgr;
    std::map<QString, QByteArray> m_files;
    QJsonObject process_files_in_json(QJsonObject json);
signals:
    void return_data();
    void streamFinished();
    void newToken(QString data);
    void newReasoningToken(QString reasoning);
public slots:
    void send_request(QJsonObject json);
    void read_data();
    void reply_finish();
};

#endif // HTTPMGR_H
