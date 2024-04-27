#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(int portNumber = 50000, QObject *parent = nullptr);

signals:
    void newClientConnected(QTcpSocket * client);
    void clientDisconnected(QTcpSocket * client);

private slots:
    void newClientConnectionReceived();
    void clientConnectionAborted();



private:
    QTcpServer * server;
    QList <QTcpSocket *> clients;

private:
    void startServer(int portNumber);
};

#endif // SERVERMANAGER_H
