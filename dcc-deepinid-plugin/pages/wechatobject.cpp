// SPDX-FileCopyrightText: 2018-2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
