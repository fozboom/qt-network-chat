#include "ChatMessageInfo.h"
#include "ui_ChatMessageInfo.h"
#include <random>

QHash<QString, QColor> ChatMessageInfo::userColorMap;

ChatMessageInfo::ChatMessageInfo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatMessageInfo)
{
    ui->setupUi(this);
}

ChatMessageInfo::~ChatMessageInfo()
{
    delete ui;
}
void ChatMessageInfo::displayMessage(QString message, QString username, bool isUserMessage)
{
    qDebug() << "Displaying message:" << message << "from user:" << username << "isUserMessage:" << isUserMessage;
    setMessageColor(getUserColor(username));
    setMessageAlignment(isUserMessage);
    ui->labelMessage->setText(message);
    ui->labelTime->setText(QDateTime::currentDateTime().toString("HH:mm"));
}

QColor ChatMessageInfo::getUserColor(QString username)
{
    if (username.isEmpty())
    {
        return Qt::black;
    }
    else
    {
        if (!userColorMap.contains(username))
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 255);
            QColor newUserColor = QColor::fromRgb(dis(gen), dis(gen), dis(gen));
            userColorMap.insert(username, newUserColor);
        }

        return userColorMap.value(username);
    }
}

void ChatMessageInfo::setMessageAlignment(bool isUserMessage)
{
    if (isUserMessage)
    {
        ui->labelMessage->setAlignment(Qt::AlignRight);
    }
    else
    {
        ui->labelMessage->setAlignment(Qt::AlignLeft);
    }
}

void ChatMessageInfo::setMessageColor(QColor color)
{
    QString css = QString("color: %1").arg(color.name());
    ui->labelMessage->setStyleSheet(css);
}
