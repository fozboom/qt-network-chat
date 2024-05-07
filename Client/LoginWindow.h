#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    QString getNickname() const;
    void removeNickname(const QString &nickname);

signals:
    void loginSuccessful();
    void userNameEntered(const QString& name);

private slots:


    void on_nickname_returnPressed();

private:
    Ui::LoginWindow *ui;
    QSqlDatabase db;
    QString nickName;

    void setupDatabase();
    bool doesNicknameExist(const QString &nickname);
    void addNickname(const QString &nickname);
};

#endif // LOGINWINDOW_H
