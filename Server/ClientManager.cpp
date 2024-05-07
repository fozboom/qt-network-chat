#include "ServerClientManager.h"
#include <QDebug>
#include <QString>
ServerClientManager::ServerClientManager(QHostAddress _ip, int _port, QObject *parent)
    : QObject{parent}
    , serverIP(_ip)
    , serverPort(_port)
{
    clientSocket = new QTcpSocket(this);
    connect (clientSocket, &QTcpSocket::connected, this, &ServerClientManager::serverConnected);
    connect (clientSocket, &QTcpSocket::disconnected, this, &ServerClientManager::serverDisconnected);
    connect (clientSocket, &QTcpSocket::readyRead, this, &ServerClientManager::processIncomingData);
}

ServerClientManager::ServerClientManager(QTcpSocket *_client, QObject *parent)
    : QObject{parent}
    , clientSocket(_client)
{
    connect (clientSocket, &QTcpSocket::connected, this, &ServerClientManager::serverConnected);
    connect (clientSocket, &QTcpSocket::disconnected, this, &ServerClientManager::serverDisconnected);
    connect (clientSocket, &QTcpSocket::readyRead, this, &ServerClientManager::processIncomingData);
}



void ServerClientManager::connectToServer()
{
    clientSocket->connectToHost(serverIP, serverPort);

}

void ServerClientManager::disconnectFromServer()
{
    clientSocket->disconnectFromHost();
}

void ServerClientManager::sendTextMessage(QString message)
{
    clientSocket->write(protocol.sendTextMessage(message, getClientName(), "Server"));
}

void ServerClientManager::sendUserName(QString name)
{
    clientSocket->write(protocol.sendUserName(name));
}

void ServerClientManager::sendIsTypingIndicator()
{
    clientSocket->write(protocol.sendTypingIndicator());
}


QString ServerClientManager::getClientName() const
{
    auto id = clientSocket->property("id").toInt();
    auto name = protocol.getName().length() > 0 ? protocol.getName() : QString("Client (%1)").arg(id);
    return name;
}

QTcpSocket *ServerClientManager::getClientSocket() const
{
    return clientSocket;
}

void ServerClientManager::processIncomingData()
{
    auto data = clientSocket->readAll();
    protocol.loadMessageData(data);
    switch (protocol.getType()) {
    case ServerProtocol::TEXT_SENDING:
        emit receivedTextMessage(protocol.getMessage(), protocol.getReceiver(), getClientName());
        break;
    case ServerProtocol::NAME_SENDING: {
        auto prevName = clientSocket->property("clientName").toString();
        clientSocket->setProperty("clientName", getClientName());
        emit clientNameUpdated(prevName, getClientName());
        break;
    }
    case ServerProtocol::USER_IS_TYPING:
        emit receivedTypingIndicator();
        break;
    default:
        break;
    }
}
