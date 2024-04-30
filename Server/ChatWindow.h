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
    ~ChatWindow();
private slots:

    void clientDisconnected();

    void on_btnSend_clicked();

    void textMessageReceived(QString message);

    void onTyping();
signals:
    void clientNameChanged(QString name);
    void isTyping (QString message);


private:
    Ui::ChatWindow *ui;
    ClientManager *client;
};

#endif // CHATWINDOW_H
