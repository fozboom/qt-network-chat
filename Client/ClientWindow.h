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
    void dataReceived(QByteArray data);

    void on_btnSend_clicked();

    void on_actionConnect_triggered();

private:
    Ui::ClientWindow *ui;
    ClientManager *client;

private:
    void createMessage(const QByteArray& data, bool isMyMessage);

};
#endif // CLIENTWINDOW_H
