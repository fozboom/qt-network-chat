#include "ChatWindow.h"
#include "ui_ChatWindow.h"

ChatWindow::ChatWindow(QTcpSocket * _client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)

{
    ui->setupUi(this);
    client = new ClientManager(_client, this);

    connect(client, &ClientManager::disconnected, this, &ChatWindow::clientDisconnected);
    connect(client, &ClientManager::textMessageReceived, this, &ChatWindow::textMessageReceived);
    connect(client, &ClientManager::userNameReceived, this, &ChatWindow::clientNameChanged);
    connect(client, &ClientManager::isTyping, this, &ChatWindow::onTyping);
    connect(ui->editMessage, &QLineEdit::textChanged, client, &ClientManager::sendIsTypingIndicator);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}


void ChatWindow::clientDisconnected()
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

void ChatWindow::textMessageReceived(QString message)
{
    ui->listMessages->addItem(message);
}

void ChatWindow::onTyping()
{
    emit isTyping(QString("%1 is typing...").arg(client->name()));
}

