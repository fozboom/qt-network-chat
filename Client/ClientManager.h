#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "ClientProtocol.h"
#include <QHostAddress>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress _ip = QHostAddress::LocalHost, int _port = 8080, QObject *parent = nullptr);
    void connectToServer();
    void sendMessage(QString message, QString receiver);
    void sendUserName(QString name);
    void sendIsTypingIndicator();
signals:
    void clientConnectedToSever();
    void clientDisconnectedFromServer();

    void receivedTextMessageFromSender(QString sender, QString message);
    void userIsTyping();
    void receivedUserName(QString name);

    void connectionAcknowledged(QString myName, QStringList clients);
    void newClientConnectedToServer (QString name);
    void clientNameChanged(QString prevName, QString name);
    void clientDisconnected(QString name);
private slots:
    void readDataFromSocket();
public slots:
    void updateUserName(const QString& name);

private:
    QTcpSocket *socket;
    QHostAddress ip;
    int port;
    ClientProtocol protocol;
    QString userName;
};

#endif // CLIENTMANAGER_H
