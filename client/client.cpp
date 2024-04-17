#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);


    m_network = new Network(this);

    ui->sb_port->setMaximum(16000);  //max port
    ui->sb_port->setValue(8080);    //default port
    ui->e_ip->setText("127.0.01");  // default ip


    connect(m_network, &Network::connected, this, &Client::connectedToHost);

    connect(ui->b_connect, &QPushButton::clicked, this, &Client::connectButtonClicked);
}

Client::~Client()
{
    delete ui;
}

//when user clicked Connect on the firts page
void Client::connectButtonClicked()
{
    m_network->connectToHost(ui->e_ip->text(), ui->sb_port->value());
}

//set second page when connect is succesful
void Client::connectedToHost()
{
    ui->stackedWidget->setCurrentIndex(1);
}
