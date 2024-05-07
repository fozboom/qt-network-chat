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
    auto chatWindow = new ChatWindow(client);
    ui->tabChats->addTab(chatWindow, QString(""));


    connect(chatWindow, &ChatWindow::clientNameChanged, this, &ServerWindow::setClientName);
    connect(chatWindow, &ChatWindow::isTyping, [this](QString name) {
        this->statusBar()->showMessage(name, 800);
    });

    connect(chatWindow, &ChatWindow::textForOtherClients, server, &ServerManager::onTextForOtherClients);
}

void ServerWindow::clientDisconnected(QTcpSocket *client)
{
    ui->listClients->addItem(QString("%1 disconnected").arg(client->property("clientName").toString()));
}

void ServerWindow::setClientName(QString prevName, QString name)
{
    auto widget = qobject_cast<QWidget *>(sender());
    auto index = ui->tabChats->indexOf(widget);
    ui->tabChats->setTabText(index, name);

    if (server->clients.contains(prevName)) {
        auto clientSocket = server->clients.take(prevName);
        server->clients[name] = clientSocket;
    }

    ui->listClients->addItem(QString("%1 connected").arg(name));

    server->notifyAllClients(prevName, name);

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
    chatWindow->disconnect();
    ui->tabChats->removeTab(index);
}
