#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDialog>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    QString getNickname() const;
    void removeNickname(const QString &nickname);
    bool doesNicknameExist(const QString &nickname);
    void addNickname(const QString &nickname);
    void setNickName(const QString &newNickName);

private slots:
    void on_nickname_returnPressed();
private:
    Ui::LoginWindow *ui;
    QSqlDatabase db;
    QString nickName;
private:
    void setupDatabase();

};

#endif // LOGINWINDOW_H
