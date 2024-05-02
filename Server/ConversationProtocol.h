#ifndef CONVERSATIONPROTOCOL_H
#define CONVERSATIONPROTOCOL_H
#include <QByteArray>
#include <QString>

class ConversationProtocol
{
public:
    enum MessageType {
        USERNAME_UPDATE,
        TEXT_MESSAGE,
        TYPING_INDICATOR,
        CONNECTION_ACKNOWLEDGED,
        NEW_CLIENT_CONNECTED,
        CLIENT_DISCONNECTED
    };

    ConversationProtocol();
    QByteArray prepareMessageDataForSending(MessageType type, QString message);
    QByteArray prepareTextMessageForSending (QString message, QString receiver, QString sender);
    QByteArray prepareTypingIndicatorForSending ();
    QByteArray prepareUserNameForSending (QString name);

    QByteArray createConnectionAcknowledgementMessage(QString clientName, QStringList otherClients);
    QByteArray createNewClientConnectedMessage (QString clientName);
    QByteArray createClientDisconnectedMessage (QString clientName);

    void deserializeReceivedData(QByteArray data);


    MessageType getMessageType() const;
    void setMessageType(MessageType newMessageType);
    QString getChatMessage() const;
    void setChatMessage(const QString &newChatMessage);
    QString getUserName() const;
    void setUserName(const QString &newUserName);
    QString getMessageReceiver() const;
    void setMessageReceiver(const QString &newMessageReceiver);

private:
    MessageType messageType;
    QString chatMessage;
    QString userName;
    QString messageReceiver;

};

#endif // CONVERSATIONPROTOCOL_H
