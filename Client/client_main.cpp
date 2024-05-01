#include "LoginWindow.h"
#include "ClientWindow.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginWindow login;
    ClientWindow client;

    QObject::connect(&login, &LoginWindow::loginSuccessful, [&]() {
        login.close();
        client.show();
    });
    QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
        qDebug() << "removeNickname";
        login.removeNickname(login.getNickname());
    });

    login.show();

    return a.exec();
}
