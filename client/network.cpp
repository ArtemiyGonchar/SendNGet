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

    NetworkParser::Request request = NetworkParser::parseRequest(data);
    emitAction(request);
}

void Network::sendFile(QString path, QByteArray id, QString clientsId)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)){
        qDebug("Fail :D");
    }

    QString fileName = QFileInfo(file).fileName();
    m_socket->write(fileName.toUtf8());
    m_socket->write("\n");
    m_socket->write(id);
}

void Network::emitAction(NetworkParser::Request request)
{
    qDebug()<<"Emit action";

    switch(request.action){
    case NetworkParser::Action::assignId: emit idAvailable(request.uuid); break;
    case NetworkParser::Action::newClient: emit clientsIdAvailable(request.clientsId, request.addition); break;
    case NetworkParser::Action::disconnected: emit clientDisconnected(request.disconnectedId); break;
    }
}


