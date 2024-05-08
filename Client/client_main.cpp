#include "LoginWindow.h"
#include "ClientWindow.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginWindow login;
    ClientWindow client;

    // QObject::connect(&login, &LoginWindow::loginSuccessful, [&]() {
    //     login.close();
    //     client.updateUserNameAndNotifyServer(login.getNickname());
    //     client.connectToServer();
    //     client.setWindowTitle(login.getNickname());
    //     client.show();
    // });
    // QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
    //     login.removeNickname(login.getNickname());
    // });

    client.show();

    return a.exec();
}
