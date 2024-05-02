#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

enum MessageType {
    FILE_SENDING,
    NAME_SENDING,
    TEXT_SENDING,
    USER_IS_TYPING,
    NAME_CHANGED,
    CONNECTION_ACK,
    NEW_CLIENT,
    CLIENT_DISCONNECTED
};

class ClientProtocol {
public:

    QByteArray serializeMessageData(MessageType type, QString message);

    QByteArray sendTextMessage (QString message, QString receiver);
    QByteArray sendTypingIndicator ();
    QByteArray sendUserName (QString name);


    void loadData(QByteArray data);
    MessageType getLastReceivedType() const;
    QString getLastReceivedMessage() const;
    QString getMessageSender() const;
    QString getMessageReceiver() const;
    QString getCurrentUserName() const;
    QStringList getConnectedClients() const;
    QString getLastConnectedClientName() const;
    QString getLastDisconnectedClientName() const;

    void setCurrentUserName(const QString &newCurrentUserName);

private:
    MessageType lastReceivedType;
    QString lastReceivedMessage;
    QString messageSender;
    QString messageReceiver;
    QString currentUserName;
    QStringList connectedClients;
    QString lastConnectedClientName;
    QString lastDisconnectedClientName;

    void handleServerMessage(MessageType type, const QString& content, const QString& receiver, const QString& sender);
};
#endif // CLIENTPROTOCOL_H
