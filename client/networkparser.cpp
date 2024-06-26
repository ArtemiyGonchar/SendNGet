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
        request.uuid = idObj["userId"].toString().toUtf8(); //test toUtf8
        request.myId = idObj["userId"].toString().toUtf8();
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
                    request.clientsId<<clientId;
                }
            }
        }
        request.action = NetworkParser::newClient;
        request.addition = "CLIENTSLIST";
        qDebug()<<request.clientsId;
    }

    if(jsonObj.contains("DISCONNECTED")){
        QJsonObject idObj = jsonObj["DISCONNECTED"].toObject();
        request.disconnectedId = idObj["Id"].toString();
        qDebug()<<"disconnected: "<<request.disconnectedId;
        request.action = NetworkParser::disconnected;
    }
    return request;
}


