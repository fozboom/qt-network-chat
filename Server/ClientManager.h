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
    void disconnectFromServer();
    void sendTextMessage(QString message);
    void sendUserName(QString name);
    void sendTypingIndicator();
    QString name() const;
    QTcpSocket* getClientSocket() const;

signals:
    void serverConnected();
    void serverDisconnected();

    void receivedTextMessage(QString message, QString receiver, QString sender);
    void receivedTypingIndicator();
    void clientNameUpdated(QString prevName, QString name);
private slots:
    void processIncomingData();

private:
    QTcpSocket *clientSocket;
    QHostAddress serverIP;
    int serverPort;
    ServerProtocol protocol;
};

#endif // CLIENTMANAGER_H
