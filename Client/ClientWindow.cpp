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



void ClientWindow::onTextMessageReceived(QString sender, QString message)
{
    createMessage(sender, sender + ": " + message, false);
}




void ClientWindow::on_btnSend_clicked()
{
    processMessageAndSend();
}


void ClientWindow::setupClient()
{
    client = new ClientManager();
    ui->editMessage->setEnabled(true);
    ui->btnSend->setEnabled(true);

    connect(client, &ClientManager::serverConnected, [this] (){
        ui->sendLayout->setEnabled(true);
        qDebug() << "Connected to server";
    });
    connect(client, &ClientManager::serverDisconnected, [this] (){
        ui->editMessage->setEnabled(false);
        ui->btnSend->setEnabled(false);
        statusBar()->showMessage("You have been disconnected by the server", 10000);
        qDebug() << "Disconnected from server";
    });
    connect(client, &ClientManager::receivedTextMessage, this, &ClientWindow::onTextMessageReceived);
    connect(client, &ClientManager::receivedTypingIndicator, this, &ClientWindow::onTypingIndicatorReceived);

    connect(ui->editMessage, &QLineEdit::textChanged, client, &ClientManager::sendIsTypingIndicator);

    connect(client, &ClientManager::receivedConnectionAcknowledgement, this, &ClientWindow::onConnectionAcknowledged);
    connect(client, &ClientManager::newClientConnectedToServer, this, &ClientWindow::onNewClientConnected);
    connect(client, &ClientManager::clientDisconnected, this, &ClientWindow::onClientDisconnected);
    connect(client, &ClientManager::clientNameUpdated, this, &ClientWindow::onClientNameUpdated);

}

void ClientWindow::connectToServer()
{
    client->connectToServer();

}

void ClientWindow::handleConnectionError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    QMessageBox::critical(this, "Error", "Server not running");
    close();
}



void ClientWindow::createMessage(const QString& username, const QString& message, bool isMyMessage)
{
    auto chatMessageInfo = new ChatMessageInfo();
    chatMessageInfo->displayMessage(message, username, isMyMessage);
    auto listItemWidget = new QListWidgetItem();
    listItemWidget->setSizeHint(QSize(0,65));
    ui->messages->addItem(listItemWidget);
    if (isMyMessage) {
        listItemWidget->setBackground(QColor(227,225,225));
    }
    ui->messages->setItemWidget(listItemWidget, chatMessageInfo);
}

void ClientWindow::processMessageAndSend()
{
    auto data = ui->editMessage->text().trimmed();
    if (data.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Message cannot be empty"));
        return;
    }
    client->sendTextMessage(data, ui->receiverBox->currentText());
    ui->editMessage->setText("");

    createMessage("", data.toUtf8(), true);
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

}

void ClientWindow::onNewClientConnected(QString name)
{
    ui->receiverBox->addItem(name);
    ui->receiverBox->update();


}

void ClientWindow::onClientNameUpdated(QString prevName, QString name)
{
    for (int i = 0; i < ui->receiverBox->count(); ++i) {
        if (ui->receiverBox->itemText(i) == prevName) {
            ui->receiverBox->setItemText(i, name);

            return;
        }
    }
}

void ClientWindow::onClientDisconnected(QString name)
{
    for (int i = 0; i < ui->receiverBox->count(); ++i) {
        if (ui->receiverBox->itemText(i) == name) {
            ui->receiverBox->removeItem(i);
            return;
        }
    }
}

void ClientWindow::updateUserNameAndNotifyServer(const QString &name)
{
    client->updateUserName(name);

}

void ClientWindow::on_editMessage_returnPressed()
{
    processMessageAndSend();
}

