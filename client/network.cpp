#include "network.h"

Network::Network(QObject *parent)
    : QObject{parent}
{
    m_socket = new QTcpSocket();


    connect(m_socket, &QTcpSocket::connected,this, &Network::connectedToHost);
    connect(m_socket, &QTcpSocket::readyRead,this , &Network::readyRead);
    connect(m_socket, &QTcpSocket::bytesWritten, this, &Network::sendChunk);


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
    QString dataString = data;

    //qDebug()<<"Chunk received "<<data.length();


    //qDebug()<<data;
    //QString dataString = data;



    if (data.endsWith(":::filename")){
        //QString dataString
        dataString = dataString.split(":::")[0];

        qDebug()<<dataString<< "NAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAME";
        m_path = "C:/SendNGet/"+dataString;
    }

    if (data.endsWith(":::size")){
        m_size = dataString.split(":::")[0];
        qDebug()<<m_size<<"SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIZe";
    }

    if(data.endsWith("b")){
        //qDebug()<<data<<"ARTEM";
        // qDebug()<<data<<"ARTEM";
        data.chop(4);// remove 4 bytes in the end ":::b"
        qDebug()<<"Chunk received "<<data.length();
        //m_data += dataString.split(":::")[0].toUtf8(); // from QString to QByteArray
        m_data += data;
    }

    if(data.endsWith("END")){
        saveFile();
    }


    if (data.endsWith("json")){
        //QString dataString = data;
        dataString = dataString.split(":::")[0];

        qDebug()<<"YEAH JSON++++++";
        data = dataString.toUtf8();

        NetworkParser::Request request = NetworkParser::parseRequest(data);
        emitAction(request);
    }
    //NetworkParser::Request request = NetworkParser::parseRequest(data);
    //emitAction(request);
}

void Network::sendChunk()
{
    qDebug()<<"CHUNK SENDED YEEEEAH YEAH";
}

void Network::saveFile()
{
    //QByteArray data = QByteArray::fromHex(m_data);
    QByteArray data = m_data;
    QFile file(m_path);
    if (!file.open(QIODevice::WriteOnly)){
        qDebug()<<"GIVNO";
        return;
    }

    qint64 bytesWritten = file.write(data);
    if (bytesWritten == -1) {
        qDebug() << "NE NORM";
    } else {
        qDebug() << "NORM";
    }

    // Закрываем файл
    file.close();
}

void Network::sendFile(QString path, QByteArray id)
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
    //m_socket->write(":::size");
    m_socket->write(fileSize.toUtf8()+":::size");
    m_socket->flush();
    QThread::msleep(1);

    const int chunkSize = 4096;
    while (!file.atEnd()) {
        //count bytes?

        QByteArray buffer = file.read(chunkSize);
        m_socket->write(buffer + ":::b");
        //m_socket->write(buffer + ":::b", chunkSize + 4);
        m_socket->flush();
        QThread::msleep(1);
    }

    m_socket->write("END");
    m_socket->flush();

    file.close();

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


