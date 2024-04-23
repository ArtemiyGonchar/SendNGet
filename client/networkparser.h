#ifndef NETWORKPARSER_H
#define NETWORKPARSER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
class NetworkParser : public QObject
{
    Q_OBJECT
public:
    explicit NetworkParser(QObject *parent = nullptr);

    enum Action {
        assignId,
        connectToClient,
        newClient,
        noaction
    };

    struct Request {
        QByteArray uuid;
        Action action;
    };


    static Request parseRequest(QByteArray data);

};

#endif // NETWORKPARSER_H