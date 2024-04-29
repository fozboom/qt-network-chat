#include "ClientWindow.h"
#include "ui_ClientWindow.h"
#include <QDebug>
#include "ChatMessageInfo.h"

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



void ClientWindow::dataReceived(QByteArray data)
{
    createMessage(data, false);
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
    connect(client, &ClientManager::dataReceived, this, &ClientWindow::dataReceived);

    client->connectToServer();
}

void ClientWindow::createMessage(const QByteArray& data, bool isMyMessage)
{
    auto chatMessageInfo = std::make_unique<ChatMessageInfo>(this);
    chatMessageInfo->setMessage(data, isMyMessage);
    auto listItemWidget = new QListWidgetItem();
    listItemWidget->setSizeHint(QSize(0,65));
    ui->messages->addItem(listItemWidget);
    if (isMyMessage) {
       listItemWidget->setBackground(QColor(227,225,225));
    }
    ui->messages->setItemWidget(listItemWidget, chatMessageInfo.release());
}
