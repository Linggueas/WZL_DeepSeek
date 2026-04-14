#include "httpmgr.h"
#include "mainwindow.h"
HttpMgr::HttpMgr(QObject *parent)
    : QObject{parent}
{
    net_mgr = new QNetworkAccessManager(this);
}

void HttpMgr::send_request(QJsonObject json)
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.deepseek.com/v1/chat/completions"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(key).toUtf8());

    QNetworkReply *reply = net_mgr->post(request, QJsonDocument(json).toJson());
    connect(reply,&QNetworkReply::readyRead,this,&HttpMgr::read_data);
    connect(reply,&QNetworkReply::finished,this,&HttpMgr::reply_finish);
}

void HttpMgr::read_data()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    while (reply->canReadLine())
    {
        QByteArray line = reply->readLine();
        if (line.startsWith("data: "))
        {
            QByteArray data = line.mid(6).trimmed();
            if (data == "[DONE]") {
                emit streamFinished();   // 通知上层流式传输结束
                return;
            }
            QJsonParseError err;
            QJsonDocument doc = QJsonDocument::fromJson(data, &err);
            if (err.error != QJsonParseError::NoError) {

                continue;
            }
            QJsonObject obj = doc.object();
            if (obj.contains("choices")) {
                QJsonArray choices = obj["choices"].toArray();
                if (!choices.isEmpty()) {
                    QJsonObject firstChoice = choices[0].toObject();
                    if (firstChoice.contains("delta")) {
                        QJsonObject delta = firstChoice["delta"].toObject();

                        // 1. 处理思考过程 (reasoning_content)
                        if (delta.contains("reasoning_content")) {
                            QString reasoning = delta["reasoning_content"].toString();
                            if (!reasoning.isEmpty()) {
                                emit newReasoningToken(reasoning);
                            }
                        }
                        if (delta.contains("content")) {
                            QString content = delta["content"].toString();
                            if (!content.isEmpty()) {
                                emit newToken(content);   // 逐字发送给界面
                            }
                        }
                    }
                }
            }
        }
    }
}

void HttpMgr::reply_finish()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    reply->deleteLater();

}
