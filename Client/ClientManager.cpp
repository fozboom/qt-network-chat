#include "ClientManager.h"
#include <QDebug>

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



void ClientManager::connectToServer()
{
    socket->connectToHost(ip, port);
    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected";
    } else {
        qDebug() << "Connection failed: " << socket->errorString();
    }
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

void ClientManager::readyRead()
{
    auto data = socket->readAll();
    protocol.loadData(data);
    qDebug() << "Protocol type: " << protocol.getType();
    switch (protocol.getType()) {
    case ConversationProtocol::TEXT_SENDING:
        emit textMessageReceived(protocol.getSender(),protocol.getMessage());
        break;
    case ConversationProtocol::NAME_SENDING:
        emit userNameReceived(protocol.getName());
        break;
    case ConversationProtocol::USER_IS_TYPING:
        emit isTyping();
        break;
    case ConversationProtocol::CONNECTION_ACK:
        emit connectionACK(protocol.getMyName(), protocol.getClientNames());
        break;
    case ConversationProtocol::NEW_CLIENT:
        emit newClientConnectedToServer(protocol.getClientName());
        break;
    case ConversationProtocol::CLIENT_DISCONNECTED:
        emit clientDisconnected(protocol.getClientName());
        break;
    case ConversationProtocol::NAME_CHANGED:
        emit clientNameChanged(protocol.getPrevName(), protocol.getClientName());
        break;

    default:
        break;
    }
}
