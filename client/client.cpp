#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    connect(ui->b_connect, &QPushButton::clicked, this, &Client::connectButtonClicked);
}

Client::~Client()
{
    delete ui;
}

void Client::connectButtonClicked()
{
    connected();
}

void Client::connected()
{
    ui->stackedWidget->setCurrentIndex(1);
}
