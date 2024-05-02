#ifndef SERVERPROTOCOL_H
#define SERVERPROTOCOL_H

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QMap>

enum MessageType {
    USERNAME_UPDATE,
    TEXT_MESSAGE,
    TYPING_INDICATOR,
    CONNECTION_ACKNOWLEDGED,
    NEW_CLIENT_CONNECTED,
    CLIENT_DISCONNECTED
};

class ServerProtocol
{
public:


    QByteArray prepareMessageDataForSending(MessageType type, QString message, QString receiver, QString sender);
    QByteArray prepareTextMessageForSending(QString message, QString receiver, QString sender);
    QByteArray prepareTypingIndicatorForSending(QString sender);
    QByteArray prepareUserNameForSending(QString name);

    QByteArray createConnectionAcknowledgementMessage(QString clientName, QStringList otherClients);
    QByteArray createNewClientConnectedMessage(QString clientName);
    QByteArray createClientDisconnectedMessage(QString clientName);

    void deserializeReceivedData(QByteArray data);

    MessageType getMessageType() const;
    QString getChatMessage() const;
    QString getUserName() const;
    QString getMessageReceiver() const;

    QMap<QString, QStringList> getMessages() const;

private:
    MessageType messageType;
    QString chatMessage;
    QString userName;
    QString messageReceiver;

    QMap<QString, QStringList> messages;
};

#endif // SERVERPROTOCOL_H
