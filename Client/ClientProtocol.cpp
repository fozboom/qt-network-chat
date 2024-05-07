#include "ClientProtocol.h"
#include <QIODevice>
#include <QDebug>

ClientProtocol::ClientProtocol() {}

QByteArray ClientProtocol::serializeMessage(MessageType messageType, QString chatMessage)
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
    return serializeMessage(USER_IS_TYPING, "");
}

QByteArray ClientProtocol::sendUserName(QString name)
{
    return serializeMessage(NAME_SENDING, name);
}

void ClientProtocol::loadMessageData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_0);
    in >> type;
    qDebug() << "loadData";
    foreach (auto c, clientNames) {
        qDebug() << c << '\n';
    }
    switch(type) {
    case TEXT_SENDING:
        in >> sender >> receiver >> message;
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

QString ClientProtocol::getName() const
{
    return name;
}

void ClientProtocol::setName(const QString &newName)
{
    name = newName;
}

QString ClientProtocol::getMessage() const
{
    return message;
}

void ClientProtocol::setMessage(const QString &newMessage)
{
    message = newMessage;
}

ClientProtocol::MessageType ClientProtocol::getType() const
{
    return type;
}

void ClientProtocol::setType(MessageType newType)
{
    type = newType;
}

QString ClientProtocol::getReceiver() const
{
    return receiver;
}

QString ClientProtocol::getClientName() const
{
    return clientName;
}

QString ClientProtocol::getPrevName() const
{
    return prevName;
}

QString ClientProtocol::getMyName() const
{
    return myName;
}

QStringList ClientProtocol::getClientNames() const
{
    return clientNames;
}

QString ClientProtocol::getSender() const
{
    return sender;
}

void ClientProtocol::setMyName(const QString &newMyName)
{
    myName = newMyName;
}
