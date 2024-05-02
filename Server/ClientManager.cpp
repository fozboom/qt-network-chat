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
    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected";
    } else {
        qDebug() << "Connection failed: " << socket->errorString();
    }
}

void ClientManager::disconnectFromHost()
{
    socket->disconnectFromHost();
}

void ClientManager::sendMessage(QString message)
{
    socket->write(protocol.prepareTextMessageForSending(message, name(), "Server"));
}



void ClientManager::sendIsTypingIndicator()
{
    socket->write(protocol.prepareTypingIndicatorForSending("Server"));
}

QString ClientManager::name() const
{
    auto id = socket->property("id").toInt();
    auto name = protocol.getUserName().length() > 0 ? protocol.getUserName() : QString("Client (%1)").arg(id);
    return name;
}

QTcpSocket *ClientManager::getClient() const
{
    return socket;
}

void ClientManager::readDataFromSocket()
{
    auto data = socket->readAll();
    protocol.deserializeReceivedData(data);
    switch (protocol.getMessageType()) {
    case TEXT_MESSAGE:
        emit receivedTextMessageFromSender(protocol.getChatMessage(), protocol.getMessageReceiver(), name());
        break;
    case TYPING_INDICATOR:
        emit userIsTyping();
        break;
    default:
        break;
    }
}
