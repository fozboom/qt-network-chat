#include "ChatWindow.h"
#include "ui_ChatWindow.h"

ChatWindow::ChatWindow(QTcpSocket * _client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)

{
    ui->setupUi(this);
    client = new ServerClientManager(_client, this);

    connect(client, &ServerClientManager::disconnected, this, &ChatWindow::clientDisconnected);
    connect(client, &ServerClientManager::chatMessageReceived, this, &ChatWindow::chatMessageReceived);
    connect(client, &ServerClientManager::clientNameUpdated, this, &ChatWindow::onClientNameChanged);
}

void ChatWindow::disconnect()
{
    client->disconnectFromServer();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::clientDisconnected()
{

}

void ChatWindow::on_btnSend_clicked()
{
    auto message = ui->editMessage->text().trimmed();
    client->composeAndSendMessage(message);
    ui->editMessage->setText("");
    ui->listMessages->addItem(message);
}

void ChatWindow::chatMessageReceived(QString message, QString receiver, QString sender)
{
    if (receiver == "Server")
    {
        ui->listMessages->addItem(QString("%1: %2").arg(sender, message));
    }
    else
    {
        emit textForOtherClients(message, receiver, client->getClientName());
    }
}


void ChatWindow::onClientNameChanged(QString prevName, QString name)
{

    emit clientNameUpdated(prevName, name);
}

