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
}

ClientWindow::~ClientWindow()
{
    delete ui;
}



void ClientWindow::dataReceived(QString message)
{
    createMessage(message, false);
}




void ClientWindow::on_btnSend_clicked()
{
    auto data = ui->editMessage->text().trimmed();
    client->sendMessage(data);
    ui->editMessage->setText("");

    createMessage(data.toUtf8(), true);
}


void ClientWindow::on_actionConnect_triggered()
{
    ui->editMessage->setEnabled(true);
    ui->btnSend->setEnabled(true);
    client = new ClientManager();
 
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

    client->connectToServer();
}

void ClientWindow::createMessage(const QString& message, bool isMyMessage)
{
    auto chatMessageInfo = std::make_unique<ChatMessageInfo>(this);
    chatMessageInfo->setMessage(message, isMyMessage);
    auto listItemWidget = new QListWidgetItem();
    listItemWidget->setSizeHint(QSize(0,65));
    ui->messages->addItem(listItemWidget);
    if (isMyMessage) {
       listItemWidget->setBackground(QColor(227,225,225));
    }
    ui->messages->setItemWidget(listItemWidget, chatMessageInfo.release());
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

