#include "ChatWindow.h"
#include "ui_ChatWindow.h"

ChatWindow::ChatWindow(QTcpSocket * _client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)
    , client(_client)
{
    ui->setupUi(this);

    connect(client, &QTcpSocket::readyRead, this, &ChatWindow::dataReceived);
    connect(client, &QTcpSocket::disconnected, this, &ChatWindow::clientDisconnected);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::dataReceived()
{
    auto data = client->readAll();
    ui->listMessages->addItem(data);
}

void ChatWindow::clientDisconnected()
{
    ui->btnSend->setEnabled(false);
    ui->editMessage->setEnabled(false);
}



void ChatWindow::on_btnSend_clicked()
{
    client->write(ui->editMessage->text().trimmed().toUtf8());
    ui->editMessage->setText("");
}

