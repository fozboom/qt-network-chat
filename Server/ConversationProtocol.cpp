#include "ConversationProtocol.h"
#include <QIODevice>

ConversationProtocol::ConversationProtocol() {}

QByteArray ConversationProtocol::serializeMessageData(MessageType messageType, QString chatMessage)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_6_3);
    dataStream << messageType << chatMessage;
    return serializedData;
}

QByteArray ConversationProtocol::sendTextMessage(QString message)
{
    return serializeMessageData(TEXT_SENDING, message);
}


QByteArray ConversationProtocol::sendTypingIndicator()
{
    return serializeMessageData(USER_IS_TYPING, "");
}

QByteArray ConversationProtocol::sendUserName(QString name)
{
    return serializeMessageData(NAME_SENDING, name);
}

void ConversationProtocol::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_3);
    in >> type;

    switch(type) {
    case TEXT_SENDING:
        in >> message;
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
