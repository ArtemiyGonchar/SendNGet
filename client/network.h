#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>



class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);
    void connectToHost(QString ip, int port);

private slots:
    void connectedToHost();

private:
    QTcpSocket *m_socket;

signals:
    void connected();
};

#endif // NETWORK_H
