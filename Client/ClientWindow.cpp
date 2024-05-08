#include "ClientWindow.h"
#include "ui_ClientWindow.h"
#include <QDebug>
#include "ChatMessageInfo.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include "ClientWindow.h"
#include "ui_ClientWindow.h"
#include <QInputDialog>
#include <QDir>

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
    , loginWindow(new LoginWindow())
{
    ui->setupUi(this);

    setupClient();
    client->connectToServer();

    connect(client, &ClientManager::errorOccurred, [this](const QString &errorString) {
        loginWindow->close();
        QMessageBox::critical(this, tr("Error"), tr("Server not running. Details: ") + errorString);
        QApplication::quit();
        exit(EXIT_FAILURE);
    });

    if (loginWindow->exec() == QDialog::Rejected) {
        client->disconnectFromServer();
        QApplication::quit();
        exit(EXIT_SUCCESS);
    }

    ui->nameEdit->setText(loginWindow->getNickname());
    client->composeAndSendName(loginWindow->getNickname());
}

ClientWindow::~ClientWindow()
{
    loginWindow->removeNickname(loginWindow->getNickname());
    delete ui;
    delete loginWindow;
}


void ClientWindow::setupClient()
{
    client = new ClientManager();
    connect(client , &ClientManager::connected, this, [this](){
        ui->btnSend->setEnabled(true);
        ui->editMessage->setEnabled(true);
    });
    connect(client, &ClientManager::disconnected, this, [this](){
        ui->btnSend->setEnabled(false);
        ui->editMessage->setEnabled(false);
    });
    connect(client, &ClientManager::chatMessageReceived, this, &ClientWindow::receiveChatMessage);
    connect(client, &ClientManager::connectionAcknowledged, this, &ClientWindow::onConnectionAcknowledgement);
    connect(client, &ClientManager::newClientConnectedToServer, this, &ClientWindow::onNewClientConnectedToServer);
    connect(client, &ClientManager::clientDisconnected, this, &ClientWindow::onClientDisconnected);
    connect(client, &ClientManager::clientNameUpdated, this, &ClientWindow::onClientNameUpdated);
}




void ClientWindow::on_btnSend_clicked()
{
    processMessageAndSend();
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
    client->composeAndSendMessage(data, ui->receiverBox->currentText());
    ui->editMessage->setText("");

    createMessage("", data.toUtf8(), true);
}

void ClientWindow::receiveChatMessage(QString sender, QString message)
{
    createMessage(sender, sender + ": " + message, false);
}




void ClientWindow::onConnectionAcknowledgement(QString myName, QStringList clientsName)
{
    ui->receiverBox->clear();
    clientsName.prepend("Server");
    foreach (auto client, clientsName) {
        ui->receiverBox->addItem(client);
    }
    setWindowTitle(myName);
}

void ClientWindow::onNewClientConnectedToServer(QString clienName)
{
    ui->receiverBox->addItem(clienName);
}

void ClientWindow::onClientNameUpdated(QString prevName, QString clientName)
{
    for (int i = 0; i < ui->receiverBox->count(); ++i) {
        if (ui->receiverBox->itemText(i) == prevName) {
            ui->receiverBox->setItemText(i, clientName);
            return;
        }
    }
}

void ClientWindow::onClientDisconnected(QString clientName)
{
    for (int i = 0; i < ui->receiverBox->count(); ++i) {
        if (ui->receiverBox->itemText(i) == clientName) {
            ui->receiverBox->removeItem(i);
            return;
        }
    }
}

void ClientWindow::on_nameEdit_returnPressed()
{
    auto newName = ui->nameEdit->text().trimmed();
    if (loginWindow->doesNicknameExist(newName)) {
        QMessageBox::warning(this, tr("Error"), tr("This nickname is already taken"));
        ui->nameEdit->setText(loginWindow->getNickname());
        return;
    }

    loginWindow->removeNickname(loginWindow->getNickname());
    loginWindow->addNickname(newName);

    loginWindow->setNickName(newName);
    client->composeAndSendName(newName);
}


void ClientWindow::on_editMessage_returnPressed()
{
    processMessageAndSend();
}

