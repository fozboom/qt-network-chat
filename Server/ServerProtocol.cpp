#include "ServerProtocol.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QIODevice>

QByteArray ServerProtocol::serializeMessageData(MessageType messageType, QString chatMessage)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << messageType << chatMessage;
    return serializedData;
}

QByteArray ServerProtocol::sendTextMessage(QString message, QString receiver, QString sender)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << TEXT_SENDING << sender << receiver << message;
    return serializedData;
}


QByteArray ServerProtocol::sendTypingIndicator()
{
    return serializeMessageData(USER_IS_TYPING, "");
}

QByteArray ServerProtocol::sendUserName(QString name)
{
    return serializeMessageData(NAME_SENDING, name);
}

QByteArray ServerProtocol::setClientNameMessage(QString previousName, QString newName)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << NAME_CHANGED<< previousName << newName;
    return serializedData;
}

QByteArray ServerProtocol::setConnectionAckMessage(QString clientName, QStringList otherClients)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << CONNECTION_ACK<< clientName << otherClients;
    return serializedData;
}

QByteArray ServerProtocol::setNewClientMessage(QString clientName)
{
    return serializeMessageData(NEW_CLIENT, clientName);
}

QByteArray ServerProtocol::setClientDisconnectedMessage(QString clientName)
{
    return serializeMessageData(CLIENT_DISCONNECTED, clientName);
}

void ServerProtocol::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_0);
    qint32 _type;
    in >> _type;

    messageType = static_cast<MessageType>(_type);
    switch(messageType) {
    case TEXT_SENDING:
        in >> messageReceiver >> chatMessage;
        break;
    case NAME_SENDING:
        in >> currentName;
    default:
        break;
    }
}


MessageType ServerProtocol::getMessageType() const
{
    return messageType;
}

QString ServerProtocol::getChatMessage() const
{
    return chatMessage;
}

QString ServerProtocol::getMessageReceiver() const
{
    return messageReceiver;
}


QString ServerProtocol::getSenderName() const
{
    return senderName;
}

QString ServerProtocol::getCurrentName() const
{
    return currentName;
}

void ServerProtocol::setCurrentName(const QString &newCurrentName)
{
    currentName = newCurrentName;
}
