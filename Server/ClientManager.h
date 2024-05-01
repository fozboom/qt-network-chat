#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "ConversationProtocol.h"
#include <QHostAddress>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress _ip = QHostAddress("195.181.246.125"), int _port = 8080, QObject *parent = nullptr);
    explicit ClientManager(QTcpSocket * _client, QObject *parent = nullptr);
    void connectToServer();
    void disconnectFromHost();
    void sendMessage(QString message);
    void sendUserName(QString name);
    void sendIsTypingIndicator();
    QString name() const;
    QTcpSocket* getClient() const;

signals:
    void connected();
    void disconnected();
    //void dataReceived(QByteArray data);

    void textMessageReceived(QString message, QString receiver, QString sender);
    void isTyping();
    void nameChanged(QString prevName, QString name);
private slots:
    void readyRead();

private:
    QTcpSocket *socket;
    QHostAddress ip;
    int port;
    ConversationProtocol protocol;
};

#endif // CLIENTMANAGER_H
