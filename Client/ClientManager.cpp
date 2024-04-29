#include "ClientManager.h"
#include <QDebug>

ClientManager::ClientManager(QHostAddress _ip, int _port, QObject *parent)
    : QObject{parent}
    , ip(_ip)
    , port(_port)
{
    client = new QTcpSocket(this);
    connect (client, &QTcpSocket::connected, this, &ClientManager::connected);
    connect (client, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect (client, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
}



void ClientManager::connectToServer()
{
    client->connectToHost(ip, port);
    if (client->waitForConnected(3000)) {
        qDebug() << "Connected";
    } else {
        qDebug() << "Connection failed: " << client->errorString();
    }
}

void ClientManager::sendMessage(QString message)
{
    client->write(message.toUtf8());
}

void ClientManager::readyRead()
{
    auto data = client->readAll();
    emit dataReceived(data);
}
