#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include "network.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Client;
}
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void connectButtonClicked();
    void connectedToHost();
    void assignId(QString id);
private:
    Ui::Client *ui;

    Network *m_network;
    QString m_id;
    QStringList clients_id;
};
#endif // CLIENT_H
