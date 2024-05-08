#include "ServerClientManager.h"

#include <QDir>

ServerClientManager::ServerClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject{parent},
    ip(ip),
    port(port)
{
    socket = new QTcpSocket(this);
    setupClient();
}

ServerClientManager::ServerClientManager(QTcpSocket *client, QObject *parent)
    : QObject{parent},
    socket(client)
{
    setupClient();
}

void ServerClientManager::connectToServer()
{
    socket->connectToHost(ip, port);
}

void ServerClientManager::disconnectFromServer()
{
    socket->disconnectFromHost();
}

void ServerClientManager::composeAndSendMessage(QString message)
{
    socket->write(protocol.composeChatMessage(message, getClientName(), "Server"));
}

void ServerClientManager::composeAndSendName(QString name)
{
    socket->write(protocol.composeNameMessage(name));
}


QString ServerClientManager::getClientName() const
{
    auto id = socket->property("id").toInt();
    auto name = protocol.getNewName().length() > 0 ? protocol.getNewName() : QString("Client (%1)").arg(id);

    return name;
}



void ServerClientManager::readyRead()
{
    auto data = socket->readAll();
    protocol.parseData(data);
    switch (protocol.getMessageType()) {
    case ServerProtocol::CHAT_MESSAGE:
        emit chatMessageReceived(protocol.getChatMessage(), protocol.getMessageReceiver(), getClientName());
        break;
    case ServerProtocol::SEND_NAME:{
        auto prevName = socket->property("clientName").toString();
        socket->setProperty("clientName", getClientName());
        emit clientNameUpdated(prevName, getClientName());
        break;
    }
    default:
        break;
    }
}

void ServerClientManager::setupClient()
{
    connect(socket, &QTcpSocket::connected, this, &ServerClientManager::connected);
    connect(socket, &QTcpSocket::disconnected, this, &ServerClientManager::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &ServerClientManager::readyRead);
}


