#include "ServerManager.h"

ServerManager::ServerManager(int portNumber, QObject *parent)
    : QObject{parent}

{
    startServer(portNumber);
}

void ServerManager::newClientConnectionReceived()
{
    auto client = server->nextPendingConnection();
    clients << client;
    auto id = clients.length() - 1;
    connect(client, &QTcpSocket::disconnected, this, &ServerManager::clientConnectionAborted);
    client->setProperty("id", id);
    emit newClientConnected(client);
}

void ServerManager::clientConnectionAborted()
{
    auto client = qobject_cast<QTcpSocket *>(sender());
    clients.removeOne(client);
    emit clientDisconnected(client);
}



void ServerManager::startServer(int portNumber)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerManager::newClientConnectionReceived);
    server->listen(QHostAddress::Any, portNumber);
}
