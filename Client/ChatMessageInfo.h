#ifndef CHATMESSAGEINFO_H
#define CHATMESSAGEINFO_H

#include <QWidget>
#include <QDateTime>
#include <QtGlobal>

namespace Ui {
class ChatMessageInfo;
}

class ChatMessageInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ChatMessageInfo(QWidget *parent = nullptr);
    ~ChatMessageInfo();
    void displayMessage(QString message, QString username, bool isUserMessage);

private:
    Ui::ChatMessageInfo *ui;
    static QHash<QString, QColor> userColorMap;
    QColor getUserColor(QString username);
    void setMessageColor(QColor color);
    void setMessageAlignment(bool isUserMessage);
};

#endif // CHATMESSAGEINFO_H
