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

#include "interface/moduleinterface.h"
#include "interface/frameproxyinterface.h"

#include "syncmodel.h"
#include "syncworker.h"

#include <QObject>

namespace dcc {
namespace cloudsync {
class SyncWorker;
class SyncModel;
}
}

using namespace DCC_NAMESPACE;
class SyncModule : public QObject, public ModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID ModuleInterface_iid FILE "deepinid.json")
    Q_INTERFACES(DCC_NAMESPACE::ModuleInterface)

public:
    explicit SyncModule(FrameProxyInterface *frameProxy = nullptr, QObject *parent = nullptr);
    ~SyncModule() Q_DECL_OVERRIDE;

    virtual void initialize() Q_DECL_OVERRIDE;
    virtual const QString name() const Q_DECL_OVERRIDE;
    virtual const QString displayName() const Q_DECL_OVERRIDE;
    virtual void contentPopped(QWidget *const w) Q_DECL_OVERRIDE;
    virtual void active() Q_DECL_OVERRIDE;
    virtual void preInitialize(bool sync = false , FrameProxyInterface::PushType = FrameProxyInterface::PushType::Normal) Q_DECL_OVERRIDE;
    QStringList availPage() const Q_DECL_OVERRIDE;
    virtual void addChildPageTrans() const Q_DECL_OVERRIDE;
    // 返回插件翻译文件路径
    QString translationPath() const Q_DECL_OVERRIDE;
    // 告知控制中心插件级别（一级模块还是二级菜单）
    QString path() const Q_DECL_OVERRIDE;
    // 根据 follow 确认位置
    QString follow() const Q_DECL_OVERRIDE;

private:
    void initSearchData() override;

private:
    SyncModel  *m_model;
    SyncWorker *m_worker;
};
