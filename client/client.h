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

private:
    Ui::Client *ui;
    Network *m_network;

};
#endif // CLIENT_H
