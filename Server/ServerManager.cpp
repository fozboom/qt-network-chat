#include "ServerManager.h"

ServerManager::ServerManager(int portNumber, QObject *parent)
    : QObject{parent}

{
    startServer(portNumber);
}

void ServerManager::notifyAllClients(QString prevName, QString name)
{
    auto message = protocol.setClientNameMessage(prevName, name);
    foreach(auto c, clients)
    {
        auto clientName = c->property("clientName").toString();
        if (clientName != name)
        {
            c->write(message);
        }
    }
}

void ServerManager::onTextForOtherClients(QString message, QString receiver, QString sender)
{
    auto mes = protocol.sendTextMessage(message, receiver, sender);

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

void ServerManager::newClientConnectionReceived()
{
    auto client = server->nextPendingConnection();

    auto id = clients.count();
    auto clientName = QString("Client %1").arg(id);
    client->setProperty("id", id);
    client->setProperty("clientName", clientName);

    connect(client, &QTcpSocket::disconnected, this, &ServerManager::clientConnectionAborted);
    emit newClientConnected(client);
    if (id >= 0)
    {
        auto message = protocol.setConnectionAckMessage(clientName, clients.keys());
        client->write(message);

        auto newMessage = protocol.setNewClientMessage(clientName);

        foreach (auto c, clients) {
            c->write(newMessage);
        }
    }
    clients[clientName] = client;
}

void ServerManager::clientConnectionAborted()
{
    auto client = qobject_cast<QTcpSocket *>(sender());
    auto clientName = client->property("clientName").toString();
    clients.remove(clientName);

    auto message = protocol.setClientDisconnectedMessage(clientName);
    foreach (auto c, clients) {
        c->write(message);
    }
    emit clientDisconnected(client);
}



void ServerManager::startServer(int portNumber)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerManager::newClientConnectionReceived);
    server->listen(QHostAddress::Any, portNumber);
}
