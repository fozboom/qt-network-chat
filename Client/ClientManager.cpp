#include "ClientManager.h"

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject{parent},
    ip(ip),
    port(port)
{
    setupClient();
}

void ClientManager::connectToServer()
{
    socket->connectToHost(ip, port);
}

void ClientManager::composeAndSendMessage(QString message, QString receiver)
{
    socket->write(protocol.composeTextMessage(message, receiver));
}

void ClientManager::composeAndSendName(QString name)
{
    socket->write(protocol.composeNameMessage(name));
}


void ClientManager::disconnectFromServer()
{
    socket->disconnectFromHost();
}

void ClientManager::readyRead()
{
    auto data = socket->readAll();
    protocol.parseData(data);
    switch (protocol.getMessageType()) {
    case ClientProtocol::CHAT_MESSAGE:
        emit chatMessageReceived(protocol.getMessageSender(), protocol.getChatMessage());
        break;
    case ClientProtocol::SEND_NAME:
        emit newNameReceived(protocol.getNewName());
        break;
    case ClientProtocol::CONNECTION_ACK:
        emit connectionAcknowledged(protocol.getMyName(), protocol.getClientNames());
        break;
    case ClientProtocol::NEW_CLIENT_CONNECTED:
        emit newClientConnectedToServer(protocol.getCurrentClientName());
        break;
    case ClientProtocol::CLIENT_DISCONNECTED:
        emit clientDisconnected(protocol.getCurrentClientName());
        break;
    case ClientProtocol::UPDATE_NAME:
        emit clientNameUpdated(protocol.getPreviousName(), protocol.getCurrentClientName());
        break;
    default:
        break;
    }
}

void ClientManager::setupClient()
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            [this](QAbstractSocket::SocketError socketError) {
                Q_UNUSED(socketError)
                emit errorOccurred(socket->errorString());
            });
}




