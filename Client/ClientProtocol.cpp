#include "ClientProtocol.h"
#include <QIODevice>
#include <QDebug>

QByteArray ClientProtocol::serializeMessageData(MessageType messageType, QString chatMessage)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << messageType << chatMessage;
    return serializedData;
}

QByteArray ClientProtocol::sendTextMessage (QString message, QString receiver)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << TEXT_SENDING << receiver << message;
    return ba;
}


QByteArray ClientProtocol::sendTypingIndicator()
{
    return serializeMessageData(USER_IS_TYPING, "");
}

QByteArray ClientProtocol::sendUserName(QString name)
{
    return serializeMessageData(NAME_SENDING, name);
}

void ClientProtocol::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_0);
    in >> lastReceivedType;
    qDebug() << "loadData";
    switch(lastReceivedType) {
    case TEXT_SENDING:
        in >> messageSender >> lastReceivedMessage >> lastReceivedMessage;
        break;
    case NAME_SENDING:
        in >> messageSender;
        break;
           break;
    case CONNECTION_ACK:
        in >> currentUserName >> connectedClients;
        break;
    case NEW_CLIENT:
        in >> currentUserName;
        break;
    case CLIENT_DISCONNECTED:
        in >> currentUserName;
        break;

    default:
        break;
    }
}
MessageType ClientProtocol::getLastReceivedType() const
{
    return lastReceivedType;
}

QString ClientProtocol::getLastReceivedMessage() const
{
    return lastReceivedMessage;
}

QString ClientProtocol::getMessageSender() const
{
    return messageSender;
}

QString ClientProtocol::getMessageReceiver() const
{
    return messageReceiver;
}

QString ClientProtocol::getCurrentUserName() const
{
    return currentUserName;
}

QStringList ClientProtocol::getConnectedClients() const
{
    return connectedClients;
}

QString ClientProtocol::getLastConnectedClientName() const
{
    return lastConnectedClientName;
}

QString ClientProtocol::getLastDisconnectedClientName() const
{
    return lastDisconnectedClientName;
}

void ClientProtocol::setCurrentUserName(const QString &newCurrentUserName)
{
    currentUserName = newCurrentUserName;
}


