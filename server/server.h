#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "network.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

private slots:
    //void newClientArrived(QByteArray id);

private:
    Network *m_network;

signals:
};

#endif // SERVER_H
