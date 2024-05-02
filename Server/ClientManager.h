#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "ServerProtocol.h"
#include <QHostAddress>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress _ip = QHostAddress::LocalHost, int _port = 8080, QObject *parent = nullptr);
    explicit ClientManager(QTcpSocket * _client, QObject *parent = nullptr);
    void connectToServer();
    void disconnectFromHost();
    void sendMessage(QString message);
    void sendUserName(QString name);
    void sendIsTypingIndicator();
    QString name() const;
    QTcpSocket* getClient() const;
signals:
    void clientConnectedToServer();
    void clientDisconnectedFromServer();
    void receivedTextMessageFromSender(QString message, QString receiver, QString sender);
    void sendClientName(QString name);
    void userIsTyping();
private slots:
    void readDataFromSocket();
private:
    QTcpSocket *socket;
    QHostAddress ip;
    int port;
    ServerProtocol protocol;
};

#endif // CLIENTMANAGER_H
