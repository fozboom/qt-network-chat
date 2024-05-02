#include "ServerManager.h"

ServerManager::ServerManager(int portNumber, QObject *parent)
    : QObject{parent}

{
    startServer(portNumber);
}


void ServerManager::onMessageForClients(QString message, QString receiver, QString sender)
{
    auto mes = protocol.prepareTextMessageForSending(message, receiver, sender);

    foreach(auto c, clients)
    {
        auto clientName = c->property("clientName").toString();
        if (clientName == receiver)
        {
            c->write(mes);
            return;
        }
    }
}

void ServerManager::onNewClientConnected()
{
    auto client = server->nextPendingConnection();

    auto id = clients.count();
    auto clientName = QString("Client %1").arg(id);
    client->setProperty("id", id);
    client->setProperty("clientName", clientName);

    connect(client, &QTcpSocket::disconnected, this, &ServerManager::onClientDisconnected);
    emit newClientConnected(client);
    if (id >= 0)
    {
        auto message = protocol.createClientDisconnectedMessage(clientName);
        client->write(message);

        auto newMessage = protocol.createNewClientConnectedMessage(clientName);

        foreach (auto c, clients) {
            c->write(newMessage);
        }
    }
    clients[clientName] = client;

}

void ServerManager::onClientDisconnected()
{
    auto client = qobject_cast<QTcpSocket *>(sender());
    auto clientName = client->property("clientName").toString();
    clients.remove(clientName);

    auto message = protocol.createClientDisconnectedMessage(clientName);
    foreach (auto c, clients) {
        c->write(message);
    }
    emit clientDisconnected(client);
}



void ServerManager::startServer(int portNumber)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerManager::onNewClientConnected);
    server->listen(QHostAddress::Any, portNumber);
}

void ServerManager::disconnectClient(QTcpSocket *client, const QString &reason)
{
    auto message = protocol.prepareTextMessageForSending(reason, client->property("clientName").toString(), "Server");
    client->write(message);
}
