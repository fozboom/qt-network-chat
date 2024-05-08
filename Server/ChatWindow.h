#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include "ServerClientManager.h"

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

    void chatMessageReceived(QString message, QString receiver, QString sender);

    void onClientNameChanged(QString prevName, QString name);
signals:
    void clientNameUpdated(QString prevName, QString name);
    void textForOtherClients(QString message, QString receiver, QString sender);


private:
    Ui::ChatWindow *ui;
    ServerClientManager *client;
};

#endif // CHATWINDOW_H
