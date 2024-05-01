#include "ConversationProtocol.h"
#include <QIODevice>
#include <QDataStream>

ConversationProtocol::ConversationProtocol() {}

QByteArray ConversationProtocol::serializeMessageData(MessageType messageType, QString chatMessage)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << messageType << chatMessage;
    return serializedData;
}

QByteArray ConversationProtocol::sendTextMessage(QString message, QString receiver)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << TEXT_SENDING << receiver << message;
    return serializedData;
}


QByteArray ConversationProtocol::sendTypingIndicator()
{
    return serializeMessageData(USER_IS_TYPING, "");
}

QByteArray ConversationProtocol::sendUserName(QString name)
{
    return serializeMessageData(NAME_SENDING, name);
}

QByteArray ConversationProtocol::setClientNameMessage(QString previousName, QString newName)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << NAME_CHANGED<< previousName << newName;
    return serializedData;
}

QByteArray ConversationProtocol::setConnectionAckMessage(QString clientName, QStringList otherClients)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << CONNECTION_ACK<< clientName << otherClients;
    return serializedData;
}

QByteArray ConversationProtocol::setNewClientMessage(QString clientName)
{
    return serializeMessageData(NEW_CLIENT, clientName);
}

QByteArray ConversationProtocol::setClientDisconnectedMessage(QString clientName)
{
    return serializeMessageData(CLIENT_DISCONNECTED, clientName);
}

void ConversationProtocol::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_0);
    qint8 _type;
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

QString ConversationProtocol::getName() const
{
    return name;
}

void ConversationProtocol::setName(const QString &newName)
{
    name = newName;
}

QString ConversationProtocol::getMessage() const
{
    return message;
}

void ConversationProtocol::setMessage(const QString &newMessage)
{
    message = newMessage;
}

ConversationProtocol::MessageType ConversationProtocol::getType() const
{
    return type;
}

void ConversationProtocol::setType(MessageType newType)
{
    type = newType;
}

QString ConversationProtocol::getReceiver() const
{
    return receiver;
}
