#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include "network.h"
//#include <QFileDialog>
//#include <QFileInfo>
#include <QString>

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

    //void connectToClient();

    void assignId(QString id);

    void addClientsToUi(QStringList clients, QString info);
    void removeClientFromUi(QString id);
    void selectClientAndSend();

    void askPage();
    void responceYes();
    void responceNo();

    void senderReceived(QString id);
    void filenameReceived (QString filename);
    void filesizeReceived (QString filesize);

    void sendFile();
    void setFile();
private:
    Ui::Client *ui;

    QString m_path;
    Network *m_network;
    QString m_id;
    QStringList clients_id;
};
#endif // CLIENT_H
