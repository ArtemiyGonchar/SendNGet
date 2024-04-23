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

void Network::sentToClientId(QByteArray id)
{
    //QJsonObject json;
    //json["userId"] = QString(id);
    //json["clientsId"] = "client`s id";

    /*QJsonObject innerJson{
                          {"userId", QString(id)},
        {"clientsId", "clientiki i ID"}
    };*/

    QJsonObject innerJson{
        {"userId", QString(id)}
    };

    QJsonObject json{
        {"ID", innerJson}
    };

    QJsonDocument jDoc { json };
    m_clients[id]->write(jDoc.toJson());
    m_clients[id]->flush();
}

void Network::sendToAllClientsId()
{

    QJsonArray clientsArray;

    for (QByteArray id : m_clients.keys()){
        clientsArray.append(QString(id));
    }

    QJsonObject innerJson{
        {"clientsId", clientsArray}
    };
    QJsonObject json{
        {"CLIENTSID", innerJson}
    };

    QJsonDocument jDoc { json };


    for (QByteArray id : m_clients.keys()){
        QByteArray data = jDoc.toJson();
        m_clients[id]->write(data);
        m_clients[id]->flush();
    }

}

void Network::clientConnected()
{
    QTcpSocket *client = m_server->nextPendingConnection();
    QByteArray id = QUuid::createUuid().toByteArray();
    m_clients[id] = client;

    qDebug()<<("New client connected");
    qDebug()<<m_clients;

    for (QByteArray id : m_clients.keys()){
        qDebug()<<(QString("Client: %1").arg(id));
    }

    //qDebug()<<m_clients.keys();

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
