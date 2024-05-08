#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H

#include <QByteArray>
#include <QString>
#include <QStringList>


class ClientProtocol
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

    ClientProtocol();

    QByteArray composeTextMessage(QString message, QString receiver);
    QByteArray composeNameMessage(QString name);

    void parseData(QByteArray data);

    const QString &getChatMessage() const;
    const QString &getNewName() const;
    MessageType getMessageType() const;
    QString getMessageReceiver() const;
    const QString &getCurrentClientName() const;
    const QString &getPreviousName() const;
    const QStringList &getClientNames() const;
    const QString &getMyName() const;
    QString getMessageSender() const;
private:
    QByteArray prepareData(MessageType type, QString data);
private:
    MessageType messageType;
    QString chatMessage;
    QString newName;
    QString messageReceiver;
    QString currentClientName;
    QString previousName;
    QStringList clientNames;
    QString myName;
    QString messageSender;


};

#endif // CLIENTPROTOCOL_H
