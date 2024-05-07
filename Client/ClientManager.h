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


    void sendTextMessage(QString message, QString receiver);
    void sendUserName(QString name);
    void sendIsTypingIndicator();
    void updateProtocolUserName(QString name);
signals:
    void serverConnected();
    void serverDisconnected();

    void receivedTextMessage(QString sender, QString message);
    void receivedTypingIndicator();
    void receivedUserName(QString name);

    void receivedConnectionAcknowledgement(QString myName, QStringList clients);
    void newClientConnectedToServer (QString name);
    void clientNameUpdated(QString prevName, QString name);
    void clientDisconnected(QString name);
    void errorOccurred(QAbstractSocket::SocketError socketError);
private slots:
    void processIncomingData();
public slots:
    void updateUserName(const QString& name);

private:
    QTcpSocket *socket;
    QHostAddress serverIP;
    int serverPort;
    ClientProtocol protocol;
    QString userName;
};

#endif // CLIENTMANAGER_H
