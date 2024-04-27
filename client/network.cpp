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
    //QString dataString = data;

    if (data.endsWith("json")){
        QString dataString = data;
        dataString = dataString.split(":::")[0];

        qDebug()<<"YEAH JSON++++++";
        data = dataString.toUtf8();

        NetworkParser::Request request = NetworkParser::parseRequest(data);
        emitAction(request);
    }
    //NetworkParser::Request request = NetworkParser::parseRequest(data);
    //emitAction(request);
}

void Network::sendFile(QString path, QByteArray id, QString clientsId)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)){
        qDebug("Fail :D");
    }

    QString fileName = QFileInfo(file).fileName();
    //m_socket->write(fileName.toUtf8());
    //m_socket->write("::");
    //m_socket->flush();
    m_socket->write(id+":::id");
    m_socket->flush();
    QThread::msleep(1);

    m_socket->write(fileName.toUtf8()+":::filename");
    m_socket->flush();
    QThread::msleep(1);
    quint64 size = file.size();
    QString fileSize = QString::number(size);
    //m_socket->write(size);
    //m_socket->write(reinterpret_cast<const char*>(&size), sizeof(size));
    //m_socket->waitForReadyRead()
    m_socket->write(fileSize.toUtf8()+":::size");
    m_socket->flush();


    //m_socket->flush();
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


