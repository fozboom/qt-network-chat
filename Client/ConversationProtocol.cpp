#include "ConversationProtocol.h"
#include <QIODevice>
#include <QDebug>

ConversationProtocol::ConversationProtocol() {}

QByteArray ConversationProtocol::serializeMessageData(MessageType messageType, QString chatMessage)
{
    QByteArray serializedData;
    QDataStream dataStream(&serializedData, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << messageType << chatMessage;
    return serializedData;
}

QByteArray ConversationProtocol::sendTextMessage (QString message, QString receiver)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << TEXT_SENDING << receiver << message;
    return ba;
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
    in.setVersion(QDataStream::Qt_5_0);
    in >> type;
    qDebug() << "loadData";
    switch(type) {
    case TEXT_SENDING:
        in >> receiver >> message;
        break;
    case NAME_SENDING:
        in >> name;
        break;
    case NAME_CHANGED:
        in >> prevName >> clientName;
        break;
    case CONNECTION_ACK:
        in >> myName >> clientNames;
        break;
    case NEW_CLIENT:
        in >> clientName;
        break;
    case CLIENT_DISCONNECTED:
        in >> clientName;
        break;

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

QString ConversationProtocol::getClientName() const
{
    return clientName;
}

QString ConversationProtocol::getPrevName() const
{
    return prevName;
}

QString ConversationProtocol::getMyName() const
{
    return myName;
}

QStringList ConversationProtocol::getClientNames() const
{
    return clientNames;
}
