#ifndef SERVERPROTOCOL_H
#define SERVERPROTOCOL_H
#include <QByteArray>
#include <QString>

class ServerProtocol
{
public:
    enum MessageType {
        NAME_SENDING,
        TEXT_SENDING,
        USER_IS_TYPING,
        NAME_CHANGED,
        CONNECTION_ACK,
        NEW_CLIENT,
        CLIENT_DISCONNECTED
    };

    ServerProtocol();
    QByteArray serializeMessage(MessageType type, QString message);
    QByteArray sendTextMessage (QString message, QString receiver, QString sender);
    QByteArray sendTypingIndicator ();
    QByteArray sendUserName (QString name);

    QByteArray setClientNameMessage (QString previousName, QString newName);
    QByteArray setConnectionAckMessage (QString clientName, QStringList otherClients);
    QByteArray setNewClientMessage (QString clientName);
    QByteArray setClientDisconnectedMessage (QString clientName);

    void loadMessageData(QByteArray data);

    QString getName() const;
    void setName(const QString &newName);

    QString getMessage() const;
    void setMessage(const QString &newMessage);

    MessageType getType() const;
    void setType(MessageType newType);

    QString getReceiver() const;

private:
    MessageType type;
    QString message;
    QString name;
    QString receiver;

};

#endif // SERVERPROTOCOL_H
