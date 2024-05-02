#include "ClientWindow.h"
#include "ui_ClientWindow.h"
#include <QDebug>
#include "ChatMessageInfo.h"
#include <QLineEdit>
#include <QMessageBox>
ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    ui->btnSend->setEnabled(false);
    ui->editMessage->setEnabled(false);
    setupClient();
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::connectToServer()
{
    client->connectToServer();
}



void ClientWindow::onMessageReceived(QString sender, QString message)
{
    createMessage(sender + ": " + message, false);
}




void ClientWindow::on_btnSend_clicked()
{
    processAndSendMessage();
}


void ClientWindow::setupClient()
{
    client = new ClientManager();
    ui->editMessage->setEnabled(true);
    ui->btnSend->setEnabled(true);

    connect(client, &ClientManager::clientConnectedToSever, [this] (){
        ui->sendLayout->setEnabled(true);
        qDebug() << "Connected to server";
    });
    connect(client, &ClientManager::clientDisconnectedFromServer, [this] (){
        ui->editMessage->setEnabled(false);
        ui->btnSend->setEnabled(false);
        statusBar()->showMessage("You have been disconnected by the server", 10000);
        qDebug() << "Disconnected from server";
    });
    connect(client, &ClientManager::receivedTextMessageFromSender, this, &ClientWindow::onMessageReceived);
    connect(client, &ClientManager::userIsTyping, this, &ClientWindow::onTypingIndicatorReceived);

    connect(ui->editMessage, &QLineEdit::textChanged, client, &ClientManager::sendIsTypingIndicator);

    connect(client, &ClientManager::connectionAcknowledged, this, &ClientWindow::onConnectionAcknowledged);
    connect(client, &ClientManager::newClientConnectedToServer, this, &ClientWindow::onNewClientConnectedToServer);
    connect(client, &ClientManager::clientDisconnected, this, &ClientWindow::onClientDisconnectedFromServer);
}



void ClientWindow::createMessage(const QString& message, bool isMyMessage)
{
    auto chatMessageInfo = new ChatMessageInfo();
    chatMessageInfo->setMessage(message, isMyMessage);
    auto listItemWidget = new QListWidgetItem();
    listItemWidget->setSizeHint(QSize(0,65));
    ui->messages->addItem(listItemWidget);
    if (isMyMessage) {
       listItemWidget->setBackground(QColor(227,225,225));
    }
    ui->messages->setItemWidget(listItemWidget, chatMessageInfo);
}


void ClientWindow::onTypingIndicatorReceived()
{
    statusBar()->showMessage("Server is typing...", 800);
}




void ClientWindow::onConnectionAcknowledged(QString myName, QStringList clients)
{
    ui->receiverBox->clear();
    clients.prepend("Server");
    foreach (auto cl, clients) {
        ui->receiverBox->addItem(cl);
    }
    setWindowTitle(myName);
}

void ClientWindow::onNewClientConnectedToServer(QString name)
{
    qDebug() << "New client connected to server: " << name;
    ui->receiverBox->addItem(name);
    ui->receiverBox->update();
}



void ClientWindow::onClientDisconnectedFromServer(QString name)
{
    for (int i = 0; i < ui->receiverBox->count(); ++i) {
        if (ui->receiverBox->itemText(i) == name) {
            ui->receiverBox->removeItem(i);
            return;
        }
    }
}


void ClientWindow::processAndSendMessage()
{
    auto data = ui->editMessage->text().trimmed();
    if (data.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Message cannot be empty"));
        return;
    }
    client->sendMessage(data, ui->receiverBox->currentText());
    ui->editMessage->setText("");

    createMessage(data.toUtf8(), true);
}

void ClientWindow::on_editMessage_returnPressed()
{
    processAndSendMessage();
}

void ClientWindow::updateAndSendUserName(const QString &name)
{
    client->updateUserName(name);
}

