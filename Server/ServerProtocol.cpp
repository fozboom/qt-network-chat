#include "ServerProtocol.h"
#include <QIODevice>
#include <QDataStream>

ServerProtocol::ServerProtocol() {}

QByteArray ServerProtocol::serializeMessage(MessageType messageType, QString chatMessage)
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
    return serializeMessage(USER_IS_TYPING, "");
}

QByteArray ServerProtocol::sendUserName(QString name)
{
    return serializeMessage(NAME_SENDING, name);
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
    return serializeMessage(NEW_CLIENT, clientName);
}

QByteArray ServerProtocol::setClientDisconnectedMessage(QString clientName)
{
    return serializeMessage(CLIENT_DISCONNECTED, clientName);
}

void ServerProtocol::loadMessageData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_0);
    qint32 _type;
    in >> _type;

    type = static_cast<MessageType>(_type);
    switch(type) {
    case TEXT_SENDING:
        in >> receiver >> message;
        break;
    case NAME_SENDING:
        in >> name;
    default:
        break;
    }
}

QString ServerProtocol::getName() const
{
    return name;
}

void ServerProtocol::setName(const QString &newName)
{
    name = newName;
}

QString ServerProtocol::getMessage() const
{
    return message;
}

void ServerProtocol::setMessage(const QString &newMessage)
{
    message = newMessage;
}

ServerProtocol::MessageType ServerProtocol::getType() const
{
    return type;
}

void ServerProtocol::setType(MessageType newType)
{
    type = newType;
}

QString ServerProtocol::getReceiver() const
{
    return receiver;
}
