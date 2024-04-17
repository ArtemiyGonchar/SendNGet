#include "network.h"

Network::Network(QObject *parent)
    : QObject{parent}
{
    m_server = new QTcpServer(this);

    connect(m_server, &QTcpServer::newConnection, this, &Network::clientConnected);
}

bool Network::portStart(int port)
{
    bool started = m_server->listen(QHostAddress::Any, port);

    if(m_server->isListening()){
        qDebug()<<"Server started";
    }

    if (started == false){
        qDebug()<<"Server error";
    }

    return started;
}

void Network::clientConnected()
{
    QTcpSocket *client = m_server->nextPendingConnection();
    QByteArray id = QUuid::createUuid().toByteArray();
    m_clients[id] = client;

    qDebug()<<("New client connected");

    //connect(client, &QTcpSocket::readyRead, this, &Network::?);
    connect(client, &QTcpSocket::disconnected, this, &Network::clientDisconnected);
    emit onClientArrived(id);
}

void Network::clientDisconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    for(QByteArray id : m_clients.keys()){
        if (m_clients[id] == client){
            qDebug()<<"Client disconnected";
            emit onClientDisconnected(id);
            m_clients.remove(id);
        }
    }

}
