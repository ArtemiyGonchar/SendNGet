#ifndef NETWORKPARSER_H
#define NETWORKPARSER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStringList>
#include <QJsonArray>

class NetworkParser : public QObject
{
    Q_OBJECT
public:
    explicit NetworkParser(QObject *parent = nullptr);

    enum Action {
        assignId,
        connectToClient,
        newClient,
        noaction,
        disconnected
    };

    struct Request {
        QByteArray uuid;
        Action action;
        QStringList clientsId;
        QString addition;
        QString disconnectedId;
    };


    static Request parseRequest(QByteArray data);

};

#endif // NETWORKPARSER_H
