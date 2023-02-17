#include "wechatobject.h"
#include <QDebug>

WeChatObject::WeChatObject(QObject *parent) : QObject(parent)
{

}

void WeChatObject::bindResult(bool bSuccess)
{
    qInfo() << "bind result:" << bSuccess;
    Q_EMIT finish();
}
