#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include "ServerProtocol.h"

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(ushort port = 4500, QObject *parent = nullptr);
    void informClientsAboutNameChange(QString prevName, QString name);
    QMap<QString, QTcpSocket *> _clients;
public slots:
    void onTextForOtherClients(QString message, QString receiver, QString sender);
signals:
    void newClientConnected(QTcpSocket *client);
    void clientDisconnected(QTcpSocket *client);
private slots:
    void onNewClientConnection();
    void onClientDisconnected();

private:
    QTcpServer *server;
    ServerProtocol protocol;
private:
    void setupServer(ushort port);
};

#endif // SERVERMANAGER_H
