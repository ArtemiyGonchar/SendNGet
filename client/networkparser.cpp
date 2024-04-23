#include "networkparser.h"

NetworkParser::NetworkParser(QObject *parent)
    : QObject{parent}
{
}

NetworkParser::Request NetworkParser::parseRequest(QByteArray data)
{
    NetworkParser::Request request;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject jsonObj = jsonDoc.object();


    if (jsonObj.contains("ID")){
        QJsonObject idObj = jsonObj["ID"].toObject();
        qDebug()<<idObj;
        qDebug()<<"===";
        qDebug()<<idObj["userId"].toString();
        request.uuid = idObj["userId"].toString().toUtf8(); //test toUtf8
        request.action = Action::assignId;
    }

    if(jsonObj.contains("CLIENTSID")){
        QJsonObject idObj = jsonObj["CLIENTSID"].toObject();
        QJsonValue clientsIdValue = idObj.value("clientsId");

        if (clientsIdValue.isArray()) {
            QJsonArray clientsArray = clientsIdValue.toArray();
            for (const QJsonValue& clientIdValue : clientsArray) {
                if (clientIdValue.isString()) {
                    QString clientId = clientIdValue.toString();
                    qDebug() << "Client ID:" << clientId;
                    request.clientsId<<clientId;
                }
            }
        }
        request.action = NetworkParser::newClient;
        request.addition = "CLIENTSLIST";
        qDebug()<<request.clientsId;
    }
    //QJsonObject json = QJsonDocument::fromJson(data).object();
    //qDebug()<<json;
    //qDebug()<<"=====";
    //request.action = parseAction(json.value("action").toString());
    //qDebug()<<"p_id==";
    //p_id = request.uuid;
    //qDebug()<<p_id;
    return request;
}


