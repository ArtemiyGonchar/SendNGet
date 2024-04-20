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

    qDebug()<<data;

    //processRawRequest(data);
}

//data analyze
void Network::processRawRequest(QByteArray rawRequest)
{
    qDebug()<<"rawreq";

    NetworkParser::Request request = NetworkParser::parseRequest(rawRequest);
    qDebug() << "=========";
    qDebug() << request.action;
}

