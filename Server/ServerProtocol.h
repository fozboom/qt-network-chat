#ifndef SERVERPROTOCOL_H
#define SERVERPROTOCOL_H

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QMap>

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

class ServerProtocol
{
public:


    QByteArray serializeMessageData(MessageType type, QString message);
    QByteArray sendTextMessage (QString message, QString receiver, QString sender);
    QByteArray sendTypingIndicator ();
    QByteArray sendUserName (QString name);

    QByteArray setClientNameMessage (QString previousName, QString newName);
    QByteArray setConnectionAckMessage (QString clientName, QStringList otherClients);
    QByteArray setNewClientMessage (QString clientName);
    QByteArray setClientDisconnectedMessage (QString clientName);

    void loadData(QByteArray data);

    MessageType getMessageType() const;
    QString getChatMessage() const;
    QString getMessageReceiver() const;


    QString getSenderName() const;

    QString getCurrentName() const;

private:
    MessageType messageType;
    QString chatMessage;
    QString senderName;
    QString messageReceiver;
    QString currentName;

};

#endif // SERVERPROTOCOL_H
