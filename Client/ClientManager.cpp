#include "ClientManager.h"
#include <QDebug>

ClientManager::ClientManager(QHostAddress _ip, int _port, QObject *parent)
    : QObject{parent}
    , ip(_ip)
    , port(_port)
{
    socket = new QTcpSocket(this);
    connect (socket, &QTcpSocket::connected, this, &ClientManager::clientConnectedToSever);
    connect (socket, &QTcpSocket::disconnected, this, &ClientManager::clientDisconnectedFromServer);
    connect (socket, &QTcpSocket::readyRead, this, &ClientManager::readDataFromSocket);
}



void ClientManager::connectToServer()
{
    socket->connectToHost(ip, port);
    protocol.setMyName(userName);
    socket->write(protocol.sendUserName(userName));

}

void ClientManager::sendMessage(QString message, QString receiver)
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

void ClientManager::setNameInProtocol(QString name)
{
    protocol.setMyName(name);
}

void ClientManager::readyRead()
{
    auto data = socket->readAll();
    protocol.loadData(data);
    switch (protocol.getType()) {
    case ConversationProtocol::TEXT_SENDING:
        emit textMessageReceived(protocol.getSender(),protocol.getMessage());
        break;
    case USER_IS_TYPING:
        emit userIsTyping();
        break;
    case CONNECTION_ACK:
        emit connectionAcknowledged(protocol.getCurrentUserName(), protocol.getConnectedClients());
        break;
    case NEW_CLIENT:
        emit newClientConnectedToServer(userName);
        break;
    case CLIENT_DISCONNECTED:
        emit clientDisconnected(protocol.getCurrentUserName());
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
