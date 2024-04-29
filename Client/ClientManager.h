#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress _ip = QHostAddress("195.181.246.125"), int _port = 8080, QObject *parent = nullptr);
    void connectToServer();
    void sendMessage(QString message);

signals:
    void connected();
    void disconnected();
    void dataReceived(QByteArray data);
private slots:
    void readyRead();

private:
    QTcpSocket *client;
    QHostAddress ip;
    int port;
};

#endif // CLIENTMANAGER_H
