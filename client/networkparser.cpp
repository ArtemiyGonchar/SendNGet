#include "networkparser.h"

NetworkParser::NetworkParser(QObject *parent)
    : QObject{parent}
{}

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
    }
    //QJsonObject json = QJsonDocument::fromJson(data).object();
    //qDebug()<<json;
    qDebug()<<"=====";
    //request.action = parseAction(json.value("action").toString());

    return request;
}

NetworkParser::Action NetworkParser::parseAction(QString data){
    if (data == "ID") return Action::assignId; //when client first connected
    if (data == "CONNECTING") return Action::connectToClient;
    if (data == "NEWCLIENT") return Action::newClient; //when new client arrive to server
    return Action::noaction;
}
