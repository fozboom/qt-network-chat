#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "ConversationProtocol.h"

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress _ip = QHostAddress("195.181.246.125"), int _port = 8080, QObject *parent = nullptr);
    void connectToServer();


    void sendMessage(QString message, QString receiver);
    void sendUserName(QString name);
    void sendIsTypingIndicator();

signals:
    void connected();
    void disconnected();
    //void dataReceived(QByteArray data);

    void textMessageReceived(QString message);
    void isTyping();
    void userNameReceived(QString name);

    void connectionACK(QString myName, QStringList clients);
    void newClientConnectedToServer (QString name);
    void clientNameChanged(QString prevName, QString name);
    void clientDisconnected(QString name);
private slots:
    void readyRead();

private:
    QTcpSocket *socket;
    QHostAddress ip;
    int port;
    ConversationProtocol protocol;
};

#endif // CLIENTMANAGER_H
