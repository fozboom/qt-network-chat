#include "ChatMessageInfo.h"
#include "ui_ChatMessageInfo.h"

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

void ChatMessageInfo::setMessage(QString message, bool isMyMessage)
{
    if (isMyMessage)
    {
        ui->labelMessage->setAlignment(Qt::AlignRight);
    }
    else
    {
        ui->labelMessage->setAlignment(Qt::AlignLeft);
    }
    ui->labelMessage->setText(message);
    ui->labelTime->setText(QDateTime::currentDateTime().toString("HH:mm"));
}
