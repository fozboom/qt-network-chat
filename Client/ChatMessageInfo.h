#ifndef CHATMESSAGEINFO_H
#define CHATMESSAGEINFO_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class ChatMessageInfo;
}

class ChatMessageInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ChatMessageInfo(QWidget *parent = nullptr);
    ~ChatMessageInfo();

    void setMessage(QString message, bool isMyMessage);

private:
    Ui::ChatMessageInfo *ui;
};

#endif // CHATMESSAGEINFO_H
