/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     justforlxz <justforlxz@outlook.com>
 *
 * Maintainer: justforlxz <justforlxz@outlook.com>
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
#include "pages/logininfo.h"
#include "pages/logininfodetail.h"
#include <DFloatingButton>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QStackedLayout;
class QLabel;
QT_END_NAMESPACE

namespace dcc {
namespace cloudsync {
class SyncModel;
enum SyncType : int;
enum SyncState : int;
}
}

namespace DCC_NAMESPACE {
class MainWindow;
namespace sync {
class LoginPage;
class IndexPage;
class LogoutPage;
class SyncWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SyncWidget(QWidget *parent = nullptr);
    ~SyncWidget();
    void setModel(dcc::cloudsync::SyncModel *model);

private:
    void initUI();
    void initConnect();

Q_SIGNALS:
    void requestLoginUser() const;

    void requestAsyncLogoutUser() const;
    void requestSetFullname(const QString &fullname);

    void requestBindAccount(const QString &uuid, const QString &hostName);
    void requestUnBindAccount(const QString &ubid);
    void requestSetAutoSync(bool enable) const;
    void requestSetModuleState(const QString &syncType, bool state);
    void requestLocalBindCheck(const QString &uosid, const QString &uuid);

    void requestUOSID(QString &uosid);
    void requestUUID(QString &uuid);
    void requestHostName(QString &hostName);

private:
    void onUserInfoChanged(const QVariantMap &userInfo);

private:
    MainWindow *m_pMainWindow;
    QStackedLayout *m_mainLayout;
    dcc::cloudsync::SyncModel *m_model;

    LoginPage *m_loginPage;
    LoginInfoPage *m_loginInfoPage;
    LoginInfoDetailPage *m_loginInfoDetailPage;
    DBackgroundGroup *m_groundGroup;

    bool m_isLogind;
};

} // namespace sync
} // namespace DCC_NAMESPACE
