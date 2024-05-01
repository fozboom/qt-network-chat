#include "ClientWindow.h"
#include "ui_ClientWindow.h"
#include <QDebug>
#include "ChatMessageInfo.h"
#include <QLineEdit>

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



void ClientWindow::dataReceived(QString sender, QString message)
{
    createMessage(sender + ": " + message, false);
}




void ClientWindow::on_btnSend_clicked()
{
    auto data = ui->editMessage->text().trimmed();
    client->sendMessage(data, ui->receiverBox->currentText());
    ui->editMessage->setText("");

    createMessage(data.toUtf8(), true);
}


void ClientWindow::setupClient()
{
    client = new ClientManager();
    ui->editMessage->setEnabled(true);
    ui->btnSend->setEnabled(true);

    connect(client, &ClientManager::connected, [this] (){
        ui->sendLayout->setEnabled(true);
        qDebug() << "Connected to server";
    });
    connect(client, &ClientManager::disconnected, [this] (){
        ui->sendLayout->setEnabled(false);
        qDebug() << "Disconnected from server";
    });
    connect(client, &ClientManager::textMessageReceived, this, &ClientWindow::dataReceived);
    connect(client, &ClientManager::isTyping, this, &ClientWindow::actionOnTypingIndicator);

    connect(ui->editMessage, &QLineEdit::textChanged, client, &ClientManager::sendIsTypingIndicator);

    connect(client, &ClientManager::connectionACK, this, &ClientWindow::onConnectionACK);
    connect(client, &ClientManager::newClientConnectedToServer, this, &ClientWindow::onNewClientConnectedToServer);
    connect(client, &ClientManager::clientDisconnected, this, &ClientWindow::onClientDisconnected);
    connect(client, &ClientManager::clientNameChanged, this, &ClientWindow::onClientNameChanged);
}

void ClientWindow::on_actionConnect_triggered()
{
    client->connectToServer();
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


void ClientWindow::actionOnTypingIndicator()
{
    statusBar()->showMessage("Server is typing...", 800);
}


void ClientWindow::on_userNameEdit_returnPressed()
{
    auto userName = ui->userNameEdit->text().trimmed();
    client->sendUserName(userName);
}

void ClientWindow::onConnectionACK(QString myName, QStringList clients)
{
    ui->receiverBox->clear();
    clients.prepend("All");
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

void ClientWindow::onClientNameChanged(QString prevName, QString name)
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

