#include "ClientProtocol.h"
#include <QJsonArray>

QByteArray ClientProtocol::prepareMessageDataForSending(MessageType messageType, QString chatMessage, QString receiver, QString sender)
{
    QJsonObject message;
    message["type"] = static_cast<int>(messageType);
    message["content"] = chatMessage;
    message["receiver"] = receiver;
    message["sender"] = sender;
    return QJsonDocument(message).toJson(QJsonDocument::Compact);
}

QByteArray ClientProtocol::prepareTextMessageForSending(QString message, QString receiver, QString sender)
{
    return prepareMessageDataForSending(TEXT_MESSAGE, message, receiver, sender);
}

QByteArray ClientProtocol::prepareTypingIndicatorForSending(QString sender)
{
    return prepareMessageDataForSending(TYPING_INDICATOR, "", "", sender);
}

QByteArray ClientProtocol::prepareConnectionMessage(QString userName)
{
    QJsonObject message;
    message["type"] = static_cast<int>(CONNECTION_ACKNOWLEDGED);
    message["currentUserName"] = userName;
    return QJsonDocument(message).toJson(QJsonDocument::Compact);
}

QByteArray ClientProtocol::prepareDisconnectionMessage(QString userName)
{
    QJsonObject message;
    message["type"] = static_cast<int>(CLIENT_DISCONNECTED);
    message["disconnectedClientName"] = userName;
    return QJsonDocument(message).toJson(QJsonDocument::Compact);
}

QByteArray ClientProtocol::prepareUserNameSending(QString userName)
{
    QJsonObject message;
    message["type"] = static_cast<int>(USER_NAME);
    message["userName"] = userName;
    return QJsonDocument(message).toJson(QJsonDocument::Compact);
}

void ClientProtocol::deserializeReceivedData(QByteArray data)
{
    QJsonObject json = QJsonDocument::fromJson(data).object();
    MessageType type = static_cast<MessageType>(json["type"].toInt());
    lastReceivedType = type;
    switch(type) {
    case TEXT_MESSAGE:
        lastReceivedMessage = json["content"].toString();
        messageReceiver = json["receiver"].toString();
        messageSender = json["sender"].toString();
        break;
    case CONNECTION_ACKNOWLEDGED:
        currentUserName = json["currentUserName"].toString();
        connectedClients = json["connectedClients"].toString().split(",");
        break;
    case NEW_CLIENT_CONNECTED:
        lastConnectedClientName = json["connectedClientName"].toString();
        break;
    case CLIENT_DISCONNECTED:
        lastDisconnectedClientName= json["disconnectedClientName"].toString();
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


