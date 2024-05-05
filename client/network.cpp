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
    QByteArray data = m_socket->read(4100);
    QString dataString = data;

    if (data.contains("responce:::ask")){
        qDebug()<<"RECPONCE ASK====================";

        emit askReceived();
    }

    if (dataString.endsWith(":::clientid")){
        dataString = dataString.split(":::")[0];
        qDebug()<<"Received sender id" << dataString;
        emit idAskReceived(m_myId);
    }

    if (data.endsWith("NO")){
        //
    }

    if (data.endsWith(":::filename")){
        //QString dataString
        dataString = dataString.split(":::")[0];

        qDebug()<<dataString<< " - File name received";

        QString path = QDir::currentPath();
        QString choppedPath;
        for (int i = 0; i < path.indexOf("SendNGet") + 8; i++){
            choppedPath += path[i];
        }
        choppedPath += "/";
        qDebug() << choppedPath;
        m_path = choppedPath + dataString;
        //m_path = "C:/SendNGet/"+dataString;
        emit filenameAskReceived(dataString);
    }

    if (data.endsWith(":::size")){
        m_size = dataString.split(":::")[0];
        qDebug()<<m_size<<" - File size received";
        emit filesizeAskReceived(m_size);
    }

    if(data.endsWith(":::b")){
        data.chop(4);// remove 4 bytes in the end ":::b"
        qDebug()<<"Chunk received "<<data.length();
        m_data += data;
    }

    if(data.endsWith("END")){
        //saveFile();
    }

    if (data.endsWith("json")){
        dataString = dataString.split(":::")[0];

        qDebug()<<"Json file received";
        data = dataString.toUtf8();

        NetworkParser::Request request = NetworkParser::parseRequest(data);
        emitAction(request);
    }

    if (data.endsWith(":::jsonuser")){
        dataString = dataString.split(":::")[0];

        qDebug()<<"Json file received";
        data = dataString.toUtf8();

        NetworkParser::Request request = NetworkParser::parseRequest(data);
        m_myId = request.myId;
        emitAction(request);
    }
}

void Network::saveFile()
{
    QByteArray data = m_data;
    QFile file(m_path);
    if (!file.open(QIODevice::WriteOnly)){
        qDebug()<<"File open error";
        return;
    }

    qint64 bytesWritten = file.write(data);
    if (bytesWritten == -1) {
        qDebug() << "Bytes error";
    } else {
        qDebug() << "Bytes wrote";
    }


    file.close();
}

void Network::sendMyId()
{
    qDebug()<< "Send my id:" << m_myId;
    m_socket->write(m_myId.toUtf8() + ":::clientid");
    m_socket->flush();
    QThread::msleep(1);
}

void Network::sendId(QByteArray id)
{
    m_socket->write(id+":::toConnectId");
    m_socket->flush();
    QThread::msleep(1);
}

void Network::sendFileName(QString filename)
{
    m_socket->write(filename.toUtf8()+":::filename");
    m_socket->flush();
    QThread::msleep(1);
}

void Network::sendFileSize(QString filesize)
{
    m_socket->write(filesize.toUtf8()+":::size");
    m_socket->flush();
    QThread::msleep(1);
}

void Network::sendFileAsk()
{
    m_socket->write("responce:::ask");
    m_socket->flush();
    QThread::msleep(1);
}

void Network::sendChunk(QByteArray data)
{
    m_socket->write(data + ":::b");
    m_socket->flush();
    QThread::msleep(1);    
}

void Network::sendFile(QString path, QByteArray id)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)){
        qDebug("Failed to open file");
    }

    QString fileName = QFileInfo(file).fileName();
    quint64 size = file.size();
    QString filesize = QString::number(size);

    sendId(id); // always must sended first
    sendFileAsk();
    sendMyId();
    sendFileName(fileName);
    sendFileSize(filesize);
    //sendFileAsk();

    const int chunkSize = 4096;
    while (!file.atEnd()) {

        QByteArray buffer = file.read(chunkSize);
        sendChunk(buffer);
    }

    m_socket->write("END");
    m_socket->flush();

    file.close();
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


