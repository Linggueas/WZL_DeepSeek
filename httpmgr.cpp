#include "httpmgr.h"
#include "mainwindow.h"
HttpMgr::HttpMgr(QObject *parent)
    : QObject{parent}
{
    net_mgr = new QNetworkAccessManager(this);
}

void HttpMgr::set_files(const std::map<QString, QByteArray> &files)
{
    m_files = files;
}

QJsonObject HttpMgr::process_files_in_json(QJsonObject json)
{
    if (m_files.empty()) {
        return json;
    }

    QJsonArray messages = json["messages"].toArray();
    if (messages.isEmpty()) {
        return json;
    }

    // 获取最后一条用户消息
    QJsonObject last_message = messages.last().toObject();
    if (last_message["role"].toString() != "user") {
        return json;
    }

    QString content = last_message["content"].toString();
    
    // 添加文件内容到消息中
    QString file_content;
    for (const auto &file : m_files) {
        QString file_name = file.first;
        QByteArray file_data = file.second;
        
        // 尝试将文件内容转换为文本
        QString text_content = QString::fromUtf8(file_data);
        if (text_content.isNull()) {
            // 如果不是UTF-8编码，使用Latin1
            text_content = QString::fromLatin1(file_data);
        }
        
        // 限制文件内容长度，避免超过API限制
        if (text_content.length() > 10000) {
            text_content = text_content.left(10000) + "...（文件内容过长，已截断）";
        }
        
        file_content += QString("\n[文件：%1]\n%2\n").arg(file_name).arg(text_content);
    }

    // 更新最后一条消息的内容
    if (!file_content.isEmpty()) {
        content += file_content;
        last_message["content"] = content;
        messages[messages.size() - 1] = last_message;
        json["messages"] = messages;
    }

    return json;
}

void HttpMgr::send_request(QJsonObject json)
{
    // 处理文件内容
    QJsonObject processed_json = process_files_in_json(json);
    
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.deepseek.com/v1/chat/completions"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg("sk-36d1b631abfd46f18e95ab66aaf210fd").toUtf8());

    QNetworkReply *reply = net_mgr->post(request, QJsonDocument(processed_json).toJson());
    connect(reply,&QNetworkReply::readyRead,this,&HttpMgr::read_data);
    connect(reply,&QNetworkReply::finished,this,&HttpMgr::reply_finish);
}

void HttpMgr::read_data()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    bool done = false;
    while (reply->canReadLine())
    {
        QByteArray line = reply->readLine();
        if (line.startsWith("data: "))
        {
            QByteArray data = line.mid(6).trimmed();
            if (data == "[DONE]") {
                done = true;
                continue;
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
    if (done) {
        emit streamFinished();   // 通知上层流式传输结束
    }
}

void HttpMgr::reply_finish()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    reply->deleteLater();

}
