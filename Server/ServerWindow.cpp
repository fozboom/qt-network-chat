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
    ui->listClients->addItem(QString("New Client added: %1").arg(id));
    auto chatWidget= new ChatWindow(client, ui->tabChats);
    ui->tabChats->addTab(chatWidget, QString("Client (%1)").arg(id));

    connect(chatWidget, &ChatWindow::clientNameUpdated, this, &ServerWindow::updateClientName);
    connect(chatWidget, &ChatWindow::textForOtherClients, server, &ServerManager::onTextForOtherClients);
}

void ServerWindow::clientDisconnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->listClients->addItem(QString("Client with id %1 disconnected").arg(id));
}

void ServerWindow::updateClientName(QString prevName, QString name)
{
    auto widget = qobject_cast<QWidget *>(sender());
    auto index = ui->tabChats->indexOf(widget);
    ui->tabChats->setTabText(index, name);

    if (server->_clients.contains(prevName)) {
        auto clientSocket = server->_clients.take(prevName);
        server->_clients[name] = clientSocket;
    }

    server->informClientsAboutNameChange(prevName, name);
}

void ServerWindow::setupServerConfiguration()
{
    server = new ServerManager();
    connect(server, &ServerManager::newClientConnected, this, &ServerWindow::newClientConnected);
    connect(server, &ServerManager::clientDisconnected, this, &ServerWindow::clientDisconnected);
}

void ServerWindow::on_tabChats_tabCloseRequested(int index)
{
    auto chatWidget = qobject_cast<ChatWindow *>(ui->tabChats->widget(index));
    chatWidget->disconnect();
    ui->tabChats->removeTab(index);
}
