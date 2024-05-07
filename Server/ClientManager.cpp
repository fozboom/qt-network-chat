#include "ClientManager.h"
#include <QDebug>
#include <QString>
ClientManager::ClientManager(QHostAddress _ip, int _port, QObject *parent)
    : QObject{parent}
    , serverIP(_ip)
    , serverPort(_port)
{
    clientSocket = new QTcpSocket(this);
    connect (clientSocket, &QTcpSocket::connected, this, &ClientManager::serverConnected);
    connect (clientSocket, &QTcpSocket::disconnected, this, &ClientManager::serverDisconnected);
    connect (clientSocket, &QTcpSocket::readyRead, this, &ClientManager::processIncomingData);
}

ClientManager::ClientManager(QTcpSocket *_client, QObject *parent)
    : QObject{parent}
    , clientSocket(_client)
{
    connect (clientSocket, &QTcpSocket::connected, this, &ClientManager::serverConnected);
    connect (clientSocket, &QTcpSocket::disconnected, this, &ClientManager::serverDisconnected);
    connect (clientSocket, &QTcpSocket::readyRead, this, &ClientManager::processIncomingData);
}



void ClientManager::connectToServer()
{
    clientSocket->connectToHost(serverIP, serverPort);
}

void ClientManager::disconnectFromServer()
{
    clientSocket->disconnectFromHost();
}

void ClientManager::sendTextMessage(QString message)
{
    clientSocket->write(protocol.sendTextMessage(message, name(), "Server"));
}

void ClientManager::sendUserName(QString name)
{
    clientSocket->write(protocol.sendUserName(name));
}

void ClientManager::sendTypingIndicator()
{
    clientSocket->write(protocol.sendTypingIndicator());
}

QString ClientManager::name() const
{
    auto name = protocol.getName();
    return name;
}

QTcpSocket *ClientManager::getClientSocket() const
{
    return clientSocket;
}

void ClientManager::processIncomingData()
{
    auto data = clientSocket->readAll();
    protocol.loadData(data);
    switch (protocol.getType()) {
    case ConversationProtocol::TEXT_SENDING:
        emit receivedTextMessage(protocol.getMessage(), protocol.getReceiver(), name());
        break;
    case ConversationProtocol::NAME_SENDING: {
        auto prevName = clientSocket->property("clientName").toString();
        clientSocket->setProperty("clientName", name());
        emit clientNameUpdated(prevName, name());
        break;
    }
    case ConversationProtocol::USER_IS_TYPING:
        emit receivedTypingIndicator();
        break;
    default:
        break;
    }
}
