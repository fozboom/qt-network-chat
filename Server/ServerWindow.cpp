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


    connect(chatWindow, &ChatWindow::clientNameChanged, this, &ServerWindow::setClientName);
    connect(chatWindow, &ChatWindow::isTyping, [this](QString name) {
            this->statusBar()->showMessage(name, 800);
        });

    connect(chatWindow, &ChatWindow::textForOtherClients, server, &ServerManager::onTextForOtherClients);
}

void ServerWindow::clientDisconnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->listClients->addItem(QString("Client with id %1 disconnected").arg(id));
}

void ServerWindow::setClientName(QString prevName, QString name)
{
    auto widget = qobject_cast<QWidget *>(sender());
    auto index = ui->tabChats->indexOf(widget);
    ui->tabChats->setTabText(index, name);

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

    chatWindow->disconnect();
    ui->tabChats->removeTab(index);
}

