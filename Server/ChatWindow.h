#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>

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
    void dataReceived();

    void clientDisconnected();

    void on_btnSend_clicked();

private:
    Ui::ChatWindow *ui;
    QTcpSocket *client;
};

#endif // CHATWINDOW_H
