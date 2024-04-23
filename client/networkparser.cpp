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
        qDebug()<<idObj;
        QJsonValue clientsId = idObj.value("clientsId");
        qDebug()<<clientsId;

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


