#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    //request = new NetworkParser(this); //?
    m_network = new Network(this);

    ui->sb_port->setMaximum(16000);  //max port
    ui->sb_port->setValue(8080);    //default port
    ui->e_ip->setText("127.0.0.1");  // default ip

    connect(m_network, &Network::connected, this, &Client::connectedToHost);
    connect(m_network, &Network::idAvailable, this, &Client::assignId);
    connect(m_network, &Network::clientsIdAvailable, this, &Client::addClientsToUi);
    connect(m_network, &Network::clientDisconnected, this, &Client::removeClientFromUi);

    connect(m_network, &Network::askReceived,this, &Client::askPage);
    //connect(m_network, &Network::idAskReceived, this, &Client)

    connect(ui->b_connect, &QPushButton::clicked, this, &Client::connectButtonClicked);
    connect(ui->b_fileSelect, &QPushButton::clicked, this, &Client::setFile);
    connect(ui->b_sendfile, &QPushButton::clicked, this, &Client::sendFile);

    connect(ui->listWidget, &QListWidget::doubleClicked, this, &Client::selectClientAndSend);
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
    qDebug()<<"Connected to host";
}

//when we recevie id this func start
void Client::assignId(QString id)
{
    qDebug() << "ID Assigned: " << id;
    m_id = id;

    ui->l_id->setText(id);
}

void Client::addClientsToUi(QStringList clients, QString info)
{
    qDebug()<<clients;
    if(info == "CLIENTSLIST"){
        for(QString id : clients){
            qDebug()<<id;
            QList<QListWidgetItem*> foundItems = ui->listWidget->findItems(id, Qt::MatchExactly);

            if (foundItems.isEmpty() && m_id != id){
                ui->listWidget->addItem(id);
            }
        }
    }
}

void Client::removeClientFromUi(QString id)
{
    QListWidgetItem *item = ui->listWidget->findItems(id, Qt::MatchExactly).first();
    ui->listWidget->takeItem(ui->listWidget->row(item));
    clients_id.removeOne(id);
}

void Client::selectClientAndSend()
{
    if(ui->listWidget->currentItem()){
        qDebug()<<"SELECTED";
    }
}

void Client::askPage()
{
    ui->stackedWidget->setCurrentIndex(2);

    connect(ui->b_yes, &QPushButton::clicked, this, &Client::responceYes);
    connect(ui->b_no, &QPushButton::clicked, this, &Client::responceNo);
    connect(m_network, &Network::idAskReceived, this, &Client::senderReceived);
    connect(m_network, &Network::filenameAskReceived, this, &Client::filenameReceived);
    connect(m_network, &Network::filesizeAskReceived, this, &Client::filesizeReceived);
}

void Client::responceYes()
{
    ui->stackedWidget->setCurrentIndex(1);
    m_network->saveFile();
}

void Client::responceNo()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Client::senderReceived(QString id)
{
    ui->l_sender->setText(id);
}

void Client::filenameReceived(QString filename)
{
    ui->l_filename_2->setText(filename);
}

void Client::filesizeReceived(QString filesize)
{
    ui->l_filesize->setText(filesize);
}

void Client::sendFile()
{
    if (!m_path.isEmpty()){
        qDebug()<<m_path;

        QString receiverId = ui->listWidget->currentItem()->text();
        m_network->sendFile(m_path, receiverId.toUtf8());
    } else {
        qDebug()<<"Path is empty!";
    }
}

void Client::setFile()
{
    if(ui->listWidget->currentItem()){
        m_path = QFileDialog::getOpenFileName(this, "Choose file", QDir::homePath());
        qDebug()<<m_path;

        QFileInfo fileInfo(m_path);
        QString fileName = fileInfo.baseName();

        ui->l_filename->setText(fileName);
    }
}
