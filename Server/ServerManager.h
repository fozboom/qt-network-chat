#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "ServerProtocol.h"

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(int portNumber = 8080, QObject *parent = nullptr);
    void disconnectClient(QTcpSocket *client, const QString& reason);
    QMap <QString, QTcpSocket *> clients;

public slots:
    void onMessageForClients(QString message, QString receiver, QString sender);
signals:
    void newClientConnected(QTcpSocket * client);
    void clientDisconnected(QTcpSocket * client);

private slots:
    void onNewClientConnected();
    void onClientDisconnected();

private:
    QTcpServer * server;

    ConversationProtocol protocol;
private:
    void startServer(int portNumber);

};

#endif // SERVERMANAGER_H
