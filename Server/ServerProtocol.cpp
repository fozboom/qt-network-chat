#include "ServerProtocol.h"

#include <QFileInfo>
#include <QIODevice>

ServerProtocol::ServerProtocol()
{

}

QByteArray ServerProtocol::composeChatMessage(QString message, QString receiver, QString sender)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << CHAT_MESSAGE << sender << receiver << message;
    return ba;
}


QByteArray ServerProtocol::composeNameMessage(QString name)
{
    return getData(SEND_NAME, name);
}


QByteArray ServerProtocol::composeUpdateNameMessage(QString prevName, QString name)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << UPDATE_NAME << prevName << name;
    return ba;
}

QByteArray ServerProtocol::composeConnectionAckMessage(QString clientName, QStringList otherClients)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << CONNECTION_ACK << clientName << otherClients;
    return ba;
}

QByteArray ServerProtocol::composeNewClientMessage(QString clientName)
{
    return getData(NEW_CLIENT_CONNECTED, clientName);
}

QByteArray ServerProtocol::composeClientDisconnectedMessage(QString clientName)
{
    return getData(CLIENT_DISCONNECTED, clientName);
}

void ServerProtocol::parseData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_0);
    in >> messageType;
    switch (messageType) {
    case CHAT_MESSAGE:
        in >> messageReceiver >> chatMessage;
        break;
    case SEND_NAME:
        in >> newName;
        break;
    default:
        break;
    }
}

QByteArray ServerProtocol::getData(MessageType type, QString data)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << type << data;
    return ba;
}

const QString &ServerProtocol::getMessageReceiver() const
{
    return messageReceiver;
}

const QString &ServerProtocol::getNewName() const
{
    return newName;
}

ServerProtocol::MessageType ServerProtocol::getMessageType() const
{
    return messageType;
}

const QString &ServerProtocol::getChatMessage() const
{
    return chatMessage;
}
