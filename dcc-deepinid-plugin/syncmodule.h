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

#include "interface/moduleobject.h"

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

class SyncModule : public ModuleObject
{
    Q_OBJECT
public:
    explicit SyncModule(QObject *parent = nullptr);
    ~SyncModule() Q_DECL_OVERRIDE;

    //active module
    void active() Q_DECL_OVERRIDE;
    //deactive module
    void deactive() override;
    //always construct new page
    QWidget *page() override;
private:
    void InitModuleData();
private:
    SyncModel  *m_model;
    SyncWorker *m_worker;
};
