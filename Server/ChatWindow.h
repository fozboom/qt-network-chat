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
    void disconnect();
    ~ChatWindow();
private slots:

    void clientDisconnected();

    void on_btnSend_clicked();

    void textMessageReceived(QString message, QString receiver);

    void onTyping();

    void onClientNameChanged(QString prevName, QString name);
signals:
    void clientNameChanged(QString prevName, QString name);
    void isTyping (QString message);
    void textForOtherClients(QString message, QString receiver, QString sender);


private:
    Ui::ChatWindow *ui;
    ClientManager *client;
};

#endif // CHATWINDOW_H
