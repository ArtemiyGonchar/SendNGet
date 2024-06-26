#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QUuid>
#include <QString>


#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);

    bool portStart(int port);

    struct Request {
        QByteArray uuid;
        QString additionalInfo;
    };

private:
    QTcpServer *m_server;
    QMap<QByteArray, QTcpSocket*> m_clients;

    QByteArray m_clientId;

signals:
    void onClientArrived(QByteArray id);
    void onClientDisconnected(QByteArray id);

public slots:
    void sentToClientId(QByteArray id);
    void sendToAllClientsId();

private slots:
    void clientConnected();
    void clientDisconnected();
    void readFromClient();
};

#endif // NETWORK_H
