#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <ClientManager.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class ClientWindow;
}
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void dataReceived(QString message);

    void on_btnSend_clicked();

    void on_actionConnect_triggered();

    void actionOnTypingIndicator();

    void on_userNameEdit_returnPressed();

    void onConnectionACK(QString myName, QStringList clients);
    void onNewClientConnectedToServer (QString name);
    void onClientNameChanged(QString prevName, QString name);
    void onClientDisconnected(QString name);

private:
    Ui::ClientWindow *ui;
    ClientManager *client;

private:
    void createMessage(const QString &message, bool isMyMessage);

    void setupClient();
};
#endif // CLIENTWINDOW_H
