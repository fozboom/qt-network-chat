#include "ConversationProtocol.h"
#include <QIODevice>
#include <QDataStream>

ConversationProtocol::ConversationProtocol() {}

QByteArray ConversationProtocol::prepareMessageDataForSending(MessageType messageType, QString chatMessage)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << messageType << chatMessage;
    return serializedData;
}

QByteArray ConversationProtocol::prepareTextMessageForSending(QString message, QString receiver, QString sender)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << TEXT_MESSAGE << sender << receiver << message;
    return serializedData;
}


QByteArray ConversationProtocol::prepareTypingIndicatorForSending()
{
    return prepareMessageDataForSending(TYPING_INDICATOR, "");
}




QByteArray ConversationProtocol::createNewClientConnectedMessage(QString clientName)
{
    return prepareMessageDataForSending(NEW_CLIENT_CONNECTED, clientName);
}

QByteArray ConversationProtocol::createClientDisconnectedMessage(QString clientName)
{
    return prepareMessageDataForSending(CLIENT_DISCONNECTED, clientName);
}

void ConversationProtocol::deserializeReceivedData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_0);
    qint32 _type;
    in >> _type;

    messageType = static_cast<MessageType>(_type);
    switch(messageType) {
    case TEXT_MESSAGE:
        in >> messageReceiver >> chatMessage;
        break;
    case USERNAME_UPDATE:
        in >> userName;
    default:
        break;
    }
}

ConversationProtocol::MessageType ConversationProtocol::getMessageType() const
{
    return messageType;
}

void ConversationProtocol::setMessageType(MessageType newMessageType)
{
    messageType = newMessageType;
}

QString ConversationProtocol::getChatMessage() const
{
    return chatMessage;
}

void ConversationProtocol::setChatMessage(const QString &newChatMessage)
{
    chatMessage = newChatMessage;
}

QString ConversationProtocol::getUserName() const
{
    return userName;
}

void ConversationProtocol::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

QString ConversationProtocol::getMessageReceiver() const
{
    return messageReceiver;
}

void ConversationProtocol::setMessageReceiver(const QString &newMessageReceiver)
{
    messageReceiver = newMessageReceiver;
}
