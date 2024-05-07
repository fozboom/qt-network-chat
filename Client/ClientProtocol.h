
#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QDataStream>

class ClientProtocol
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

    ClientProtocol();
    QByteArray serializeMessage(MessageType type, QString message);


    QByteArray sendTextMessage (QString message, QString receiver);
    QByteArray sendTypingIndicator ();
    QByteArray sendUserName (QString name);


    void loadMessageData(QByteArray data);

    QString getName() const;
    void setName(const QString &newName);

    QString getMessage() const;
    void setMessage(const QString &newMessage);

    MessageType getType() const;
    void setType(MessageType newType);

    QString getReceiver() const;

    QString getClientName() const;

    QString getPrevName() const;

    QString getMyName() const;

    QStringList getClientNames() const;

    QString getSender() const;

    void setMyName(const QString &newMyName);

private:
    MessageType type;
    QString message;
    QString name;
    QString receiver;
    QString clientName;
    QString prevName;
    QString myName;
    QStringList clientNames;
    QString sender;

};

#endif // CLIENTPROTOCOL_H
