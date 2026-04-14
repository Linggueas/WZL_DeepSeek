#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include<QJsonArray>
class HttpMgr : public QObject
{
    Q_OBJECT
public:
    explicit HttpMgr(QObject *parent = nullptr);
private:
    QNetworkAccessManager*net_mgr;
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
