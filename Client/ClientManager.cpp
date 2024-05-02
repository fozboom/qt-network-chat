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
    if (socket->waitForConnected(3000)) {
        auto message = protocol.prepareConnectionMessage(userName);
        protocol.setCurrentUserName(userName);
        socket->write(message);
        qDebug() << "Connected";
    } else {
        qDebug() << "Connection failed: " << socket->errorString();
    }
}

void ClientManager::sendMessage(QString message, QString receiver)
{
    socket->write(protocol.prepareTextMessageForSending(message, receiver, userName));
}

void ClientManager::sendUserName(QString name)
{
    socket->write(protocol.prepareUserNameSending(name));
}


void ClientManager::sendIsTypingIndicator()
{
    socket->write(protocol.prepareTypingIndicatorForSending(userName));
}



void ClientManager::readDataFromSocket()
{
    auto data = socket->readAll();
    protocol.deserializeReceivedData(data);
    switch (protocol.getLastReceivedType()) {
    case TEXT_MESSAGE:
        emit receivedTextMessageFromSender(protocol.getMessageSender(),protocol.getLastReceivedMessage());
        break;
    case TYPING_INDICATOR:
        emit userIsTyping();
        break;
    case CONNECTION_ACKNOWLEDGED:
        emit connectionAcknowledged(protocol.getCurrentUserName(), protocol.getConnectedClients());
        break;
    case NEW_CLIENT_CONNECTED:
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
}
