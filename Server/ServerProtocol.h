#ifndef SERVERPROTOCOL_H
#define SERVERPROTOCOL_H

#include <QByteArray>
#include <QString>



class ServerProtocol
{
public:
    enum MessageType{
        CHAT_MESSAGE,
        SEND_NAME,
        UPDATE_NAME,
        CONNECTION_ACK,
        NEW_CLIENT_CONNECTED,
        CLIENT_DISCONNECTED
    };
    ServerProtocol();

    QByteArray composeChatMessage(QString message, QString receiver, QString sender);
    QByteArray composeNameMessage(QString name);

    QByteArray composeUpdateNameMessage(QString prevName, QString name);
    QByteArray composeConnectionAckMessage(QString clientName, QStringList otherClients);
    QByteArray composeNewClientMessage(QString clientName);
    QByteArray composeClientDisconnectedMessage(QString clientName);

    void parseData(QByteArray data);

    const QString &getChatMessage() const;

    const QString &getNewName() const;

    MessageType getMessageType() const;

    const QString &getMessageReceiver() const;

private:
    QByteArray getData(MessageType type, QString data);

    MessageType messageType;
    QString chatMessage;
    QString newName;
    QString messageReceiver;

};

#endif // SERVERPROTOCOL_H
