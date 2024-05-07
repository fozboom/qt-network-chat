
#include "ServerManager.h"

ServerManager::ServerManager(int portNumber, QObject *parent)
    : QObject{parent}

{
    startServer(portNumber);
}


void ServerManager::onMessageForClients(QString message, QString receiver, QString sender)
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
        auto message = protocol.setConnectionAckMessage(clientName, clients.keys());
        client->write(message);

        auto newMessage = protocol.setNewClientMessage(clientName);

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

    auto message = protocol.setClientDisconnectedMessage(clientName);
    foreach (auto c, clients) {
        c->write(message);
    }
    emit clientDisconnected(client);
}

QString ServerManager::getCurrentUserName()
{
    return protocol.getCurrentName();
}

void ServerManager::setUserNameInProtocol(QString name)
{
    protocol.setCurrentName(name);
}



void ServerManager::startServer(int portNumber)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerManager::onNewClientConnected);
    server->listen(QHostAddress::Any, portNumber);
}

void ServerManager::disconnectClient(QTcpSocket *client, const QString &reason)
{
    auto message = protocol.sendTextMessage(reason, client->property("clientName").toString(), "Server");
    client->write(message);
}

#include "moc_ServerManager.cpp"
