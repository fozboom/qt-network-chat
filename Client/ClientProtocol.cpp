#include "ClientProtocol.h"

#include <QFileInfo>
#include <QIODevice>

ClientProtocol::ClientProtocol()
{

}

QByteArray ClientProtocol::composeTextMessage(QString message, QString receiver)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << CHAT_MESSAGE << receiver << message;
    return ba;
}


QByteArray ClientProtocol::composeNameMessage(QString name)
{
    return prepareData(SEND_NAME, name);
}


void ClientProtocol::parseData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_0);
    in >> messageType;
    switch (messageType) {
    case CHAT_MESSAGE:
        in >> messageSender >>messageReceiver >> chatMessage;
        break;
    case SEND_NAME:
        in >> newName;
        break;
    case UPDATE_NAME:
        in >> previousName >> currentClientName;
        break;
    case NEW_CLIENT_CONNECTED:
    case CLIENT_DISCONNECTED:
        in >> currentClientName;
        break;
    case CONNECTION_ACK:
        in >> myName >> clientNames;
        break;
    default:
        break;
    }
}

QByteArray ClientProtocol::prepareData(MessageType type, QString data)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << type << data;
    return ba;
}

QString ClientProtocol::getMessageSender() const
{
    return messageSender;
}

const QString &ClientProtocol::getMyName() const
{
    return myName;
}

const QStringList &ClientProtocol::getClientNames() const
{
    return clientNames;
}

const QString &ClientProtocol::getPreviousName() const
{
    return previousName;
}

const QString &ClientProtocol::getCurrentClientName() const
{
    return currentClientName;
}

QString ClientProtocol::getMessageReceiver() const
{
    return messageReceiver;
}


ClientProtocol::MessageType ClientProtocol::getMessageType() const
{
    return messageType;
}


const QString &ClientProtocol::getNewName() const
{
    return newName;
}

const QString &ClientProtocol::getChatMessage() const
{
    return chatMessage;
}
