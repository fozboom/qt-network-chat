#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include "LoginWindow.h"
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

    void connectToServer();

private slots:

    void on_btnSend_clicked();

    void receiveChatMessage(QString sender, QString message);

    void onConnectionAcknowledgement(QString myName, QStringList clientsName);
    void onNewClientConnectedToServer(QString clienName);
    void onClientNameUpdated(QString prevName, QString clientName);
    void onClientDisconnected(QString clientName);

    void on_nameEdit_returnPressed();

    void on_editMessage_returnPressed();

private:
    Ui::ClientWindow *ui;
    ClientManager *client;
    LoginWindow *loginWindow;
    void setupClient();
    void createMessage(const QString& username, const QString& message, bool isMyMessage);
    void processMessageAndSend();


};
#endif // CLIENTWINDOW_H
