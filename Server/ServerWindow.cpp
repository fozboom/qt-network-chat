#include "ServerWindow.h"
#include "ui_ServerWindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    setupServerConfiguration();
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::newClientConnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->listClients->addItem(QString("Client with id %1 connected").arg(id));
    auto chatWindow = new ChatWindow(client);
    ui->tabChats->addTab(chatWindow, QString("Client %1").arg(id));

}

void ServerWindow::clientDisconnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->listClients->addItem(QString("Client with id %1 disconnected").arg(id));
}

void ServerWindow::setupServerConfiguration()
{
    server = new ServerManager();
    connect(server, &ServerManager::newClientConnected, this, &ServerWindow::newClientConnected);
    connect(server, &ServerManager::clientDisconnected, this, &ServerWindow::clientDisconnected);
}
