// SPDX-FileCopyrightText: 2018-2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <DDBusInterface>
#include <QObject>

using Dtk::Core::DDBusInterface;

class DeepinIdProxy: public QObject
{
    Q_OBJECT
public:
    explicit DeepinIdProxy(QObject *parent = nullptr);

    Q_PROPERTY(QVariantMap UserInfo READ userInfo NOTIFY UserInfoChanged)

    QVariantMap userInfo();

    void setDBusBlockSignals(bool status);

    void Logout();

    void Login();

    DDBusInterface& get_deepinid() const {
        return *this->m_deepinId;
    }

signals:
    void UserInfoChanged(const QVariantMap& value) const;

private:
    DDBusInterface *m_deepinId;
};
