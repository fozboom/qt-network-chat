#ifndef SERVERCLIENTMANAGER_H
#define SERVERCLIENTMANAGER_H

#include "ServerProtocol.h"

#include <QObject>
#include <QTcpSocket>

class ServerClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerClientManager(QHostAddress ip = QHostAddress("195.181.246.125"), ushort port = 8080, QObject *parent = nullptr);
    explicit ServerClientManager(QTcpSocket *client, QObject *parent = nullptr);

    void connectToServer();
    void disconnectFromServer();

    void composeAndSendMessage(QString message);
    void composeAndSendName(QString name);
    QString getClientName() const;
signals:
    void connected();
    void disconnected();

    void chatMessageReceived(const QString message, QString receiver, QString sender);
    void clientNameUpdated(QString prevName, QString name);

private slots:
    void readyRead();

private:
    QTcpSocket *socket;
    QHostAddress ip;
    ushort port;
    ServerProtocol protocol;
private:
    void setupClient();
};

#endif // SERVERCLIENTMANAGER_H
