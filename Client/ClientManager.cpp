#include "ClientManager.h"
#include <QDebug>

ClientManager::ClientManager(QHostAddress _ip, int _port, QObject *parent)
    : QObject{parent}
    , serverIP(_ip)
    , serverPort(_port)
{
    socket = new QTcpSocket(this);
    connect (socket, &QTcpSocket::connected, this, &ClientManager::serverConnected);
    connect (socket, &QTcpSocket::disconnected, this, &ClientManager::serverDisconnected);
    connect (socket, &QTcpSocket::readyRead, this, &ClientManager::processIncomingData);
}



void ClientManager::connectToServer()
{
    socket->connectToHost(serverIP, serverPort);
    protocol.setMyName(userName);
    socket->write(protocol.sendUserName(userName));

}

void ClientManager::sendTextMessage(QString message, QString receiver)
{
    socket->write(protocol.sendTextMessage(message, receiver));
}

void ClientManager::sendUserName(QString name)
{
    socket->write(protocol.sendUserName(name));
}

void ClientManager::sendIsTypingIndicator()
{
    socket->write(protocol.sendTypingIndicator());
}

void ClientManager::updateProtocolUserName(QString name)
{
    protocol.setMyName(name);
}

void ClientManager::processIncomingData()
{
    auto data = socket->readAll();
    protocol.loadData(data);
    switch (protocol.getType()) {
    case ConversationProtocol::TEXT_SENDING:
        emit receivedTextMessage(protocol.getSender(),protocol.getMessage());
        break;
    case ConversationProtocol::NAME_SENDING:
        emit receivedUserName(protocol.getName());
        break;
    case ConversationProtocol::USER_IS_TYPING:
        emit receivedTypingIndicator();
        break;
    case ConversationProtocol::CONNECTION_ACK:
        emit receivedConnectionAcknowledgement(protocol.getMyName(), protocol.getClientNames());
        break;
    case ConversationProtocol::NEW_CLIENT:
        emit newClientConnectedToServer(protocol.getClientName());
        break;
    case ConversationProtocol::CLIENT_DISCONNECTED:
        emit clientDisconnected(protocol.getClientName());
        break;
    case ConversationProtocol::NAME_CHANGED:
        emit clientNameUpdated(protocol.getPrevName(), protocol.getClientName());
        break;

    default:
        break;
    }
}

void ClientManager::updateUserName(const QString &name)
{
    userName = name;
    protocol.setMyName(userName);
}
