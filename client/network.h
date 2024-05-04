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
    void sendFile(QString file, QByteArray id);

private slots:

    void connectedToHost();
    void readyRead();
    void sendChunk(QByteArray data);
    void saveFile();

    void sendId(QByteArray id);
    void sendFileName(QString filename);
    void sendFileSize(QString filesize);



    void emitAction(NetworkParser::Request request);

private:
    QTcpSocket *m_socket;

    QString m_path; // path to file
    QString m_size; // file size
    QByteArray m_data; // contains chunks of bytes

signals:
    void connected();
    void idAvailable(QString id);
    void clientsIdAvailable(QStringList id, QString info);
    void clientDisconnected(QString id);
};

#endif // NETWORK_H
