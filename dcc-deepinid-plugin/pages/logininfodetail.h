/*
 * Copyright (C) 2016 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     duanhongyu <duanhongyu@uniontech.com>

 * Maintainer: duanhongyu <duanhongyu@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "interface/namespace.h"

#include "syncmodel.h"
#include "utils.h"
#include "syncstateicon.h"

#include <dtkwidget_global.h>
#include <widgets/settingsgroup.h>

DWIDGET_BEGIN_NAMESPACE
class DListView;
class DTipLabel;
DWIDGET_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QObject;
class QWidget;
class QVBoxLayout;
class QStandardItemModel;
class QStandardItem;
class QLabel;
QT_END_NAMESPACE


namespace dcc {
namespace widgets {
class SwitchWidget;
}
}

/**
 * @brief The LoginInfoDetailPage class 展示登录信息三级页面
 */
class LoginInfoDetailPage : public QWidget
{
    Q_OBJECT
public:
    enum InfoType {
        Available,
        NoService,  // 无服务
        NoNetwork,
        NoActive // 未激活
    };

public:
    explicit LoginInfoDetailPage(QWidget *parent = nullptr);
    ~LoginInfoDetailPage();

    void setModel(SyncModel *model);

    void initUI();
    void initConnection();

Q_SIGNALS:
    void requestBindAccount(const QString &uuid, const QString &hostName);
    void requestUnBindAccount(const QString &ubid);

    void requestSetAutoSync(bool enable) const;
    void requestSetModuleState(const QString &syncType, bool state);
    void requestLocalBindCheck(const QString &uuid);

    // DELETE
    void requestUOSID(QString &uosid);
    void requestUUID(QString &uuid);

public Q_SLOTS:
    void onUserInfoChanged(const QVariantMap &infos);
    void updateItemCheckStatus(const QString &name, bool visible);
    void onModuleStateChanged(std::pair<SyncType, bool> state);
    void onUserUnbindInfoChanged(const QString& ubid);
    void onBindStateChanged(const bool state);

    void onStateChanged(const std::pair<qint32, QString> &state);
    void onLastSyncTimeChanged(const qlonglong lastSyncTime);
    void onAutoSyncChanged(const bool state);

    void onResetPasswdError(const QString &error);

private:
    void updateUserBindStatus();
    void onBindUserAccountChanged(bool checked);
    void bindUserAccount();
    void unbindUserAccount();
    void showItemDisabledStatus(InfoType status);

private:
    SyncModel *m_model;

    QVBoxLayout *m_mainLayout;
    dcc::widgets::SwitchWidget *m_bindSwitch;
    dcc::widgets::SwitchWidget *m_autoSyncSwitch;
    DTipLabel *m_autoSyncTips;
    DTipLabel *m_bindedTips;

    // 同步配置项
    DTK_WIDGET_NAMESPACE::DListView *m_listView;
    QStandardItemModel *m_listModel;
    QMap<SyncType, QStandardItem *> m_itemMap;
    SyncStateIcon *m_stateIcon;
    dcc::widgets::SettingsGroup *m_group;

    // 最后同步时间
    QLabel *m_lastSyncTimeLbl;
    DTipLabel *m_autoSyncTipsBottom;
    DTipLabel *m_disabledTips;

    // 解绑ID
    QString m_ubID;

    bool m_syncState;
};
