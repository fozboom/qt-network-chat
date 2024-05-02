#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include "ClientManager.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QTcpSocket * _client, QWidget *parent = nullptr);
    void disconnectClient();
    ~ChatWindow();
    QTcpSocket* getClient() const;
private slots:
    void onClientDisconnected();
    void on_btnSend_clicked();
    void onMessageReceived(QString message, QString receiver, QString sender);
    void onTypingIndicatorReceived();
signals:
    void typingStatusChanged (QString message);
    void newMessageToBroadcast(QString message, QString receiver, QString sender);


private:
    Ui::ChatWindow *ui;
    ClientManager *client;
};

#endif // CHATWINDOW_H
