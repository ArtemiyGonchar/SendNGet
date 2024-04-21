#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include "networkparser.h"


class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);
    void connectToHost(QString ip, int port);

private slots:
    void connectedToHost();
    void readyRead();
    //void processRawRequest(QByteArray rawRequest);

private:
    QTcpSocket *m_socket;

signals:
    void connected();
    void idAvailable(QString id);
};

#endif // NETWORK_H
