#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include "networkparser.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QString>
#include <QThread>

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);
    void connectToHost(QString ip, int port);
    void sendFile(QString file, QByteArray id, QString clientsId);

private slots:

    void connectedToHost();
    void readyRead();
    void sendChunk();
    void saveFile();
    //void sendFile(QFile file);

    void emitAction(NetworkParser::Request request);

private:
    QTcpSocket *m_socket;

    QString m_path;
    QString m_size;
    QByteArray m_data;

signals:
    void connected();
    void idAvailable(QString id);
    void clientsIdAvailable(QStringList id, QString info);
    void clientDisconnected(QString id);
};

#endif // NETWORK_H
