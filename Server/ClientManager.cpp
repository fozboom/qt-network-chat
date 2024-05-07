#include "ClientManager.h"
#include <QDebug>
#include <QString>
ClientManager::ClientManager(QHostAddress _ip, int _port, QObject *parent)
    : QObject{parent}
    , ip(_ip)
    , port(_port)
{
    socket = new QTcpSocket(this);
    connect (socket, &QTcpSocket::connected, this, &ClientManager::connected);
    connect (socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect (socket, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
}

ClientManager::ClientManager(QTcpSocket *_client, QObject *parent)
    : QObject{parent}
    , socket(_client)
{
    connect (socket, &QTcpSocket::connected, this, &ClientManager::connected);
    connect (socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect (socket, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
}



void ClientManager::connectToServer()
{
    socket->connectToHost(ip, port);
}

void ClientManager::disconnectFromHost()
{
    socket->disconnectFromHost();
}

void ClientManager::sendMessage(QString message)
{
    socket->write(protocol.sendTextMessage(message, name(), "Server"));
}

void ClientManager::sendUserName(QString name)
{
    socket->write(protocol.sendUserName(name));
}

void ClientManager::sendIsTypingIndicator()
{
    socket->write(protocol.sendTypingIndicator());
}

QString ClientManager::name() const
{
    auto name = protocol.getName();
    return name;
}

QTcpSocket *ClientManager::getClient() const
{
    return socket;
}

void ClientManager::readyRead()
{
    auto data = socket->readAll();
    protocol.loadData(data);
    switch (protocol.getType()) {
    case ConversationProtocol::TEXT_SENDING:
        emit textMessageReceived(protocol.getMessage(), protocol.getReceiver(), name());
        break;
    case ConversationProtocol::NAME_SENDING: {
        auto prevName = socket->property("clientName").toString();
        socket->setProperty("clientName", name());
        emit nameChanged(prevName, name());
        break;
    }
    case ConversationProtocol::USER_IS_TYPING:
        emit isTyping();
        break;
    default:
        break;
    }
}
