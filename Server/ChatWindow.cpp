#include "ChatWindow.h"
#include "ui_ChatWindow.h"

ChatWindow::ChatWindow(QTcpSocket * _client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)

{
    ui->setupUi(this);
    client = new ClientManager(_client, this);

    connect(client, &ClientManager::sendClientName, this, &ChatWindow::onSendName);
    connect(client, &ClientManager::clientDisconnectedFromServer, this, &ChatWindow::onClientDisconnected);
    connect(client, &ClientManager::receivedTextMessageFromSender, this, &ChatWindow::onMessageReceived);
    connect(client, &ClientManager::userIsTyping, this, &ChatWindow::onTypingIndicatorReceived);
    connect(ui->editMessage, &QLineEdit::textChanged, client, &ClientManager::sendIsTypingIndicator);
}

void ChatWindow::disconnectClient()
{
    client->disconnectFromHost();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

QTcpSocket *ChatWindow::getClient() const
{
    return client->getClient();
}


void ChatWindow::onClientDisconnected()
{
    ui->btnSend->setEnabled(false);
    ui->editMessage->setEnabled(false);
}



void ChatWindow::on_btnSend_clicked()
{
    auto message = ui->editMessage->text().trimmed();
    client->sendMessage(message);
    ui->editMessage->setText("");
    ui->listMessages->addItem(message);
}

void ChatWindow::onMessageReceived(QString message, QString receiver, QString sender)
{
    if (receiver == "Server")
    {
        ui->listMessages->addItem(QString("%1: %2").arg(sender, message));
    }
    else
    {
        emit newMessageToBroadcast(message, receiver, client->name());
    }
}

void ChatWindow::onTypingIndicatorReceived()
{
    emit typingStatusChanged(QString("%1 is typing...").arg(client->name()));
}

void ChatWindow::onSendName(QString name)
{
    emit sendClientNameToTabWindow(name);
}
