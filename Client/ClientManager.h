#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ClientProtocol.h"

#include <QObject>
#include <QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress ip = QHostAddress("195.181.246.125"), ushort port = 8080, QObject *parent = nullptr);

    void connectToServer();

    void composeAndSendMessage(QString message, QString receiver);
    void composeAndSendName(QString name);
    void disconnectFromServer();

signals:
    void connected();
    void disconnected();

    void chatMessageReceived(QString sender, QString message);
    void newNameReceived(QString name);

    void connectionAcknowledged(QString myName, QStringList clientsName);
    void newClientConnectedToServer(QString clienName);
    void clientNameUpdated(QString prevName, QString clientName);
    void clientDisconnected(QString clientName);

    void errorOccurred(const QString &errorString);

private slots:
    void readyRead();

private:
    QTcpSocket *socket;
    QHostAddress ip;
    ushort port;
    ClientProtocol protocol;
private:
    void setupClient();
};

#endif // CLIENTMANAGER_H
