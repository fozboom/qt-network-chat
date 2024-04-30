#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "ConversationProtocol.h"

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress _ip = QHostAddress::LocalHost, int _port = 8080, QObject *parent = nullptr);
    explicit ClientManager(QTcpSocket * _client, QObject *parent = nullptr);
    void connectToServer();
    void sendMessage(QString message);
    void sendUserName(QString name);
    void sendIsTypingIndicator();
    QString name() const;

signals:
    void connected();
    void disconnected();
    //void dataReceived(QByteArray data);

    void textMessageReceived(QString message);
    void isTyping();
    void userNameReceived(QString name);
private slots:
    void readyRead();

private:
    QTcpSocket *socket;
    QHostAddress ip;
    int port;
    ConversationProtocol protocol;
};

#endif // CLIENTMANAGER_H
