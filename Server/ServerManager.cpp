#include "ServerManager.h"

ServerManager::ServerManager(ushort port, QObject *parent)
    : QObject{parent}
{
    setupServer(port);
}

void ServerManager::informClientsAboutNameChange(QString prevName, QString name)
{
    auto message = protocol.composeUpdateNameMessage(prevName, name);
    foreach (auto cl, _clients) {
        auto clientName = cl->property("clientName").toString();
        if (clientName != name) {
            cl->write(message);
        }
    }
}

void ServerManager::onTextForOtherClients(QString message, QString receiver, QString sender)
{
    auto msg = protocol.composeChatMessage(message, receiver, sender);
    foreach (auto cl, _clients) {
        auto clientName = cl->property("clientName").toString();
        if (clientName == receiver) {
            cl->write(msg);
            return;
        }
    }
}

void ServerManager::onNewClientConnection()
{
    auto client = server->nextPendingConnection();

    auto id = _clients.count() + 1;
    auto clientName = QString("Client (%1)").arg(id);
    client->setProperty("id", id);
    client->setProperty("clientName", clientName);

    connect(client, &QTcpSocket::disconnected, this, &ServerManager::onClientDisconnected);
    emit newClientConnected(client);

    if (id > 1) {
        auto message = protocol.composeConnectionAckMessage(clientName, _clients.keys());
        client->write(message);

        auto newClientMessage = protocol.composeNewClientMessage(clientName);
        foreach (auto cl, _clients) {
            cl->write(newClientMessage);
        }
    }
    _clients[clientName] = client;
}

void ServerManager::onClientDisconnected()
{
    auto client = qobject_cast<QTcpSocket *>(sender());
    auto clientName = client->property("clientName").toString();
    _clients.remove(clientName);
    auto message = protocol.composeClientDisconnectedMessage(clientName);
    foreach (auto cl, _clients) {
        cl->write(message);
    }

    emit clientDisconnected(client);
}

void ServerManager::setupServer(ushort port)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerManager::onNewClientConnection);
    server->listen(QHostAddress::Any, port);
}
