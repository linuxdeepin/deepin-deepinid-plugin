// SPDX-FileCopyrightText: 2018-2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "syncdbusproxy.h"

const static QString DEEPIN_ID_SERVICE = QStringLiteral("com.deepin.deepinid");
const static QString DEEPIN_ID_INTERFACE = QStringLiteral("com.deepin.deepinid");
const static QString DEEPIN_ID_PATH = QStringLiteral("/com/deepin/deepinid");

bool IntString::operator!=(const IntString &intString)
{
    return intString.state != state || intString.description != description;
}

QDBusArgument &operator<<(QDBusArgument &argument, const IntString &intString)
{
    argument.beginStructure();
    argument << intString.state << intString.description;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, IntString &intString)
{
    argument.beginStructure();
    argument >> intString.state >> intString.description;
    argument.endStructure();
    return argument;
}

void registerIntStringMetaType()
{
    qRegisterMetaType<IntString>("IntString");
    qDBusRegisterMetaType<IntString>();
}

SyncDaemon::SyncDaemon(QObject *parent)
    : QObject(parent)
    , m_syncInner(new DDBusInterface(DEEPIN_ID_SERVICE,
                                     DEEPIN_ID_PATH,
                                     DEEPIN_ID_INTERFACE,
                                     QDBusConnection::sessionBus(),
                                     this))
{
    registerIntStringMetaType();
}

void SyncDaemon::SwitcherSet(const QString &arg_0, bool state)
{
    m_syncInner->asyncCallWithArgumentList("SwitcherSet", { state });
}

QDBusPendingCall SyncDaemon::SwitcherDump()
{
    return m_syncInner->asyncCall("SwitcherDump");
}

void SyncDaemon::setDBusBlockSignals(bool status)
{
    m_syncInner->blockSignals(status);
}

qlonglong SyncDaemon::lastSyncTime()
{
    return m_syncInner->property("LastSyncTime").toLongLong();
}

IntString SyncDaemon::state()
{
    return qvariant_cast<IntString>(m_syncInner->property("State"));
}
