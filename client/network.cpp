#include "network.h"

Network::Network(QObject *parent)
    : QObject{parent}
{
    m_socket = new QTcpSocket();

    connect(m_socket, &QTcpSocket::connected,this, &Network::connectedToHost);
    connect(m_socket, &QTcpSocket::readyRead,this , &Network::readyRead);
}

void Network::connectToHost(QString ip, int port)
{
    m_socket->connectToHost(QHostAddress(ip), port);
}

void Network::connectedToHost()
{
    emit connected();
}

//reading data from socket
void Network::readyRead()
{
    QByteArray data = m_socket->readAll();

    //QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    //QJsonObject jsonObj = jsonDoc.object();
    //qDebug()<< jsonObj;

    /*if (jsonObj.contains("ID")){
        QJsonObject idObj = jsonObj["ID"].toObject();
        qDebug()<<idObj;
    }*/

    qDebug()<<"-----";
    qDebug()<<data;
    qDebug()<<"-----";
    //qDebug()<<jsonDoc;
    //processRawRequest(data);
    NetworkParser::Request request = NetworkParser::parseRequest(data);
    qDebug()<<request.uuid;
}

//data analyze
/*void Network::processRawRequest(QByteArray rawRequest)
{
    qDebug()<<"rawreq";

    NetworkParser::Request request = NetworkParser::parseRequest(rawRequest);
    qDebug() << "=========";
    //qDebug() << request.action;
}*/

