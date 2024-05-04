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
    ui->tabChats->addTab(chatWindow, server->getCurrentUserName());

    connect(chatWindow, &ChatWindow::sendClientNameToTabWindow, this, &ServerWindow::setClientName);
    connect(chatWindow, &ChatWindow::typingStatusChanged, [this](QString name) {
            this->statusBar()->showMessage(name, 800);
        });

    connect(chatWindow, &ChatWindow::newMessageToBroadcast, server, &ServerManager::onMessageForClients);
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

void ServerWindow::on_tabChats_tabCloseRequested(int index)
{
    auto chatWindow = qobject_cast<ChatWindow *> (ui->tabChats->widget(index));
    server->disconnectClient(chatWindow->getClient(), "You have been disconnected by the server.");
    chatWindow->disconnectClient();
    ui->tabChats->removeTab(index);
}

void ServerWindow::setClientName(QString name)
{
    auto widget = qobject_cast<QWidget *>(sender());
    auto index = ui->tabChats->indexOf(widget);
    ui->tabChats->setTabText(index, name);
    tabName = name;
    server->setUserNameInProtocol(tabName);
}

