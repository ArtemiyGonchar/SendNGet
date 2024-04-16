#include "network.h"

Network::Network(QObject *parent)
    : QObject{parent}
{
    m_server = new QTcpServer(this);
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
