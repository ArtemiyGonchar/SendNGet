#include "server.h"

Server::Server(QObject *parent)
    : QObject{parent}
{
    m_network = new Network(this);
    m_network->portStart(8080);
}
