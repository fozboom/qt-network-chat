#include "ChatWindow.h"
#include "ui_ChatWindow.h"

ChatWindow::ChatWindow(QTcpSocket * _client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)

{
    ui->setupUi(this);
    client = new ServerClientManager(_client, this);

    connect(client, &ServerClientManager::serverDisconnected, this, &ChatWindow::clientDisconnected);
    connect(client, &ServerClientManager::receivedTextMessage, this, &ChatWindow::textMessageReceived);
    connect(client, &ServerClientManager::clientNameUpdated, this, &ChatWindow::onClientNameChanged);
    connect(client, &ServerClientManager::receivedTypingIndicator, this, &ChatWindow::onTyping);
    connect(ui->editMessage, &QLineEdit::textChanged, client, &ServerClientManager::sendIsTypingIndicator);
}

void ChatWindow::disconnect()
{
    client->disconnectFromServer();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

QTcpSocket *ChatWindow::getClient() const
{
    return client->getClientSocket();
}


void ChatWindow::clientDisconnected()
{
    ui->btnSend->setEnabled(false);
    ui->editMessage->setEnabled(false);
}



void ChatWindow::on_btnSend_clicked()
{
    auto message = ui->editMessage->text().trimmed();
    client->sendTextMessage(message);
    ui->editMessage->setText("");
    ui->listMessages->addItem(message);
}

void ChatWindow::textMessageReceived(QString message, QString receiver, QString sender)
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

void ChatWindow::onTyping()
{
    emit isTyping(QString("%1 is typing...").arg(client->getClientName()));
}

void ChatWindow::onClientNameChanged(QString prevName, QString name)
{
    emit clientNameChanged(prevName, name);
}

