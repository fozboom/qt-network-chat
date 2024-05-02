#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

enum MessageType {
    TEXT_MESSAGE,
    USER_NAME,
    TYPING_INDICATOR,
    CONNECTION_ACKNOWLEDGED,
    NEW_CLIENT_CONNECTED,
    CLIENT_DISCONNECTED
};

class ClientProtocol {
public:

    QByteArray prepareMessage(MessageType type, const QString& content);
    void processMessage(const QByteArray& message);

    QByteArray prepareMessageDataForSending(MessageType messageType, QString chatMessage, QString receiver, QString sender);
    QByteArray prepareTextMessageForSending(QString message, QString receiver, QString sender);
    QByteArray prepareTypingIndicatorForSending(QString sender);
    QByteArray prepareConnectionMessage(QString userName);
    QByteArray prepareDisconnectionMessage(QString userName);
    QByteArray prepareUserNameSending(QString userName);
    void deserializeReceivedData(QByteArray data);

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
