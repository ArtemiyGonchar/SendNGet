#include "server.h"

Server::Server(QObject *parent)
    : QObject{parent}
{
    m_network = new Network(this);
    m_network->portStart(8080);

    connect(m_network, &Network::onClientArrived, this, &Server::newClientArrived);
}

void Server::newClientArrived(QByteArray id)
{
    qDebug()<<id;
    m_network->sentToClientId(id);
    QThread::msleep(100);
    m_network->sendToAllClientsId();
}
