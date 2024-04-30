#ifndef CONVERSATIONPROTOCOL_H
#define CONVERSATIONPROTOCOL_H
#include <QByteArray>
#include <QString>
class ConversationProtocol
{
public:
    enum MessageType {
        FILE_SENDING,
        NAME_SENDING,
        TEXT_SENDING,
        USER_IS_TYPING
    };

    ConversationProtocol();
    QByteArray serializeMessageData(MessageType type, QString message);
    QByteArray sendTextMessage (QString message);
    QByteArray sendTypingIndicator ();
    QByteArray sendUserName (QString name);

    void loadData(QByteArray data);

    QString getName() const;
    void setName(const QString &newName);

    QString getMessage() const;
    void setMessage(const QString &newMessage);

    MessageType getType() const;
    void setType(MessageType newType);

private:
    MessageType type;
    QString message;
    QString name;

};

#endif // CONVERSATIONPROTOCOL_H
