#include "ServerProtocol.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QByteArray ServerProtocol::prepareMessageDataForSending(MessageType type, QString message, QString receiver, QString sender)
{
    QJsonObject json;
    json["type"] = static_cast<int>(type);
    json["content"] = message;
    json["receiver"] = receiver;
    json["sender"] = sender;
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QByteArray ServerProtocol::prepareTextMessageForSending(QString message, QString receiver, QString sender)
{
    return prepareMessageDataForSending(TEXT_MESSAGE, message, receiver, sender);
}

QByteArray ServerProtocol::prepareTypingIndicatorForSending(QString sender)
{
    return prepareMessageDataForSending(TYPING_INDICATOR, "", "", sender);
}

QByteArray ServerProtocol::prepareUserNameForSending(QString name)
{
    return prepareMessageDataForSending(USERNAME_UPDATE, name, "", "");
}

QByteArray ServerProtocol::createConnectionAcknowledgementMessage(QString clientName, QStringList otherClients)
{
    QJsonObject json;
    json["type"] = static_cast<int>(CONNECTION_ACKNOWLEDGED);
    json["clientName"] = clientName;
    json["otherClients"] = QJsonArray::fromStringList(otherClients);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QByteArray ServerProtocol::createNewClientConnectedMessage(QString clientName)
{
    QJsonObject json;
    json["type"] = static_cast<int>(NEW_CLIENT_CONNECTED);
    json["clientName"] = clientName;
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QByteArray ServerProtocol::createClientDisconnectedMessage(QString clientName)
{
    QJsonObject json;
    json["type"] = static_cast<int>(CLIENT_DISCONNECTED);
    json["clientName"] = clientName;
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

void ServerProtocol::deserializeReceivedData(QByteArray data)
{
    QJsonObject json = QJsonDocument::fromJson(data).object();
    messageType = static_cast<MessageType>(json["type"].toInt());
    chatMessage = json["content"].toString();
    userName = json["sender"].toString();
    messageReceiver = json["receiver"].toString();

    if (messageType == TEXT_MESSAGE) {
        messages[userName].append(chatMessage);
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

QString ServerProtocol::getUserName() const
{
    return userName;
}

QString ServerProtocol::getMessageReceiver() const
{
    return messageReceiver;
}

QMap<QString, QStringList> ServerProtocol::getMessages() const
{
    return messages;
}
