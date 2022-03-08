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
#include "../utils.h"

#include <DFloatingButton>
#include <DLabel>
#include <DListView>
#include <QBoxLayout>
#include <QObject>
#include <QWidget>

namespace dcc {
namespace cloudsync {
class SyncModel;
}
}

DWIDGET_BEGIN_NAMESPACE
class DLabel;
class DLineEdit;
class DToolButton;
DWIDGET_END_NAMESPACE

namespace DCC_NAMESPACE {
namespace sync {
class DownloadUrl;
class AvatarWidget;
/**
 * @brief The LoginInfo class  登录信息
 */
class LoginInfoPage : public QWidget
{
    Q_OBJECT
public:
    explicit LoginInfoPage(QWidget *parent = nullptr);
    ~LoginInfoPage();

    void setModel(dcc::cloudsync::SyncModel *model);

Q_SIGNALS:
    void requestLogoutUser() const;
    void requestSetFullname(const QString &fullname);

protected:
    void initUI();
    void initConnection();
    void onUserInfoChanged(const QVariantMap &infos);
    bool eventFilter(QObject *obj, QEvent *event) override;
    void onEditingFinished(const QString& userFullName);
    void onAvatarChanged(const QString &avaPath);
    void onUserInfoListChanged(const QList<QPair<QString, QString>> &moduleTs);
    void setAvatarPath(const QString &avatarPath);

private:
    QVBoxLayout *m_mainLayout;
    dcc::cloudsync::SyncModel *m_model;

    DownloadUrl *m_downloader;
    QString m_avatarPath;
    AvatarWidget *m_avatar;

    DTK_WIDGET_NAMESPACE::DLabel *m_username;
    DTK_WIDGET_NAMESPACE::DToolButton *m_editNameBtn;
    DTK_WIDGET_NAMESPACE::DLineEdit *m_inputLineEdit;

    // Info ListView
    DTK_WIDGET_NAMESPACE::DListView *m_listView;
    QStandardItemModel *m_listModel;

    DTK_WIDGET_NAMESPACE::DToolButton *m_logoutBtn;
    DTK_WIDGET_NAMESPACE::DToolButton *m_editInfoBtn;
};
} // namespace sync
} // namespace DCC_NAMESPACE

