#include "ClientManager.h"
#include <QDebug>
#include <QString>
ClientManager::ClientManager(QHostAddress _ip, int _port, QObject *parent)
    : QObject{parent}
    , ip(_ip)
    , port(_port)
{
    socket = new QTcpSocket(this);
    connect (socket, &QTcpSocket::connected, this, &ClientManager::clientConnectedToServer);
    connect (socket, &QTcpSocket::disconnected, this, &ClientManager::clientDisconnectedFromServer);
    connect (socket, &QTcpSocket::readyRead, this, &ClientManager::readDataFromSocket);
}

ClientManager::ClientManager(QTcpSocket *_client, QObject *parent)
    : QObject{parent}
    , socket(_client)
{
    connect (socket, &QTcpSocket::connected, this, &ClientManager::clientConnectedToServer);
    connect (socket, &QTcpSocket::disconnected, this, &ClientManager::clientDisconnectedFromServer);
    connect (socket, &QTcpSocket::readyRead, this, &ClientManager::readDataFromSocket);
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

void ClientManager::readDataFromSocket()
{
    auto data = socket->readAll();
    protocol.loadData(data);
    switch (protocol.getMessageType()) {
    case TEXT_SENDING:
        emit receivedTextMessageFromSender(protocol.getChatMessage(), protocol.getMessageReceiver(), name());
        break;
    case USER_IS_TYPING:
        emit userIsTyping();
        break;
    case NAME_SENDING:
        emit sendClientName(protocol.getCurrentName());
    default:
        break;
    }
}
