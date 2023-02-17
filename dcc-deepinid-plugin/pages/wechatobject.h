#ifndef WECHATOBJECT_H
#define WECHATOBJECT_H

#include <QObject>

class WeChatObject : public QObject
{
    Q_OBJECT
public:
    explicit WeChatObject(QObject *parent = nullptr);

Q_SIGNALS:
    void finish();
public Q_SLOTS:
    void bindResult(bool bSuccess);
};

#endif // WECHATOBJECT_H
