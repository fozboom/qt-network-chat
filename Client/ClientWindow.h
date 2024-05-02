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
    void connectToServer();
    void processAndSendMessage();

private slots:
    void onMessageReceived(QString sender, QString message);
    void on_btnSend_clicked();
    void onTypingIndicatorReceived();
    void onConnectionAcknowledged(QString myName, QStringList clients);
    void onNewClientConnectedToServer (QString name);
    void onClientDisconnectedFromServer(QString name);
    void on_editMessage_returnPressed();
public slots:
    void updateAndSendUserName(const QString& name);
private:
    Ui::ClientWindow *ui;
    ClientManager *client;

private:
    void createMessage(const QString &message, bool isMyMessage);
    void setupClient();
};
#endif // CLIENTWINDOW_H
