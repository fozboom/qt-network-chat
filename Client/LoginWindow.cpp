#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include <QMessageBox>
LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setupDatabase();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::setupDatabase() {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("172.17.0.2");
    db.setDatabaseName("users");
    db.setUserName("postgres");
    db.setPassword("admin");

    if (!db.open()) {
        qCritical() << "Cannot open database:" << db.lastError();
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS nicknames (nickname TEXT UNIQUE)")) {
        qCritical() << "Cannot create table:" << query.lastError();
    }
}

bool LoginWindow::doesNicknameExist(const QString &nickname) {
    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM nicknames WHERE nickname = ?");
    query.addBindValue(nickname);
    if (!query.exec() || !query.next()) {
        return false;
    }
    return true;
}

void LoginWindow::addNickname(const QString &nickname) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO nicknames (nickname) VALUES (?)");
    query.addBindValue(nickname);
    if (!query.exec()) {
        qCritical() << "Cannot insert nickname:" << query.lastError();
    }
}

void LoginWindow::removeNickname(const QString &nickname)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM nicknames WHERE nickname = :nickname");
    query.bindValue(":nickname", nickname);
    if (!query.exec()) {
        qWarning() << "Не удалось удалить никнейм:" << query.lastError();
    }
}


void LoginWindow::on_nickname_returnPressed()
{
    QString nickname = ui->nickname->text();

    if (nickname.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Nickname cannot be empty"));
        return;
    }

    if (doesNicknameExist(nickname)) {
        QMessageBox::warning(this, tr("Error"), tr("This nickname is already taken"));
        return;
    }
    nickName = nickname;
    addNickname(nickname);
    emit loginSuccessful();
    emit userNameEntered(nickName);
}

QString LoginWindow::getNickname() const
{
    return nickName;
}

