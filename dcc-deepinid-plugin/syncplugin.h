// SPDX-FileCopyrightText: 2018-2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SYNCPLUGIN_H
#define SYNCPLUGIN_H
#include "interface/plugininterface.h"

using namespace DCC_NAMESPACE;
#define PLUGIN_IID "org.deepin.dde.ControlCenter.Plugin/1.4"

class SyncPlugin:public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_IID FILE "deepinid.json")
    Q_INTERFACES(DCC_NAMESPACE::PluginInterface)
public:
    explicit SyncPlugin(QObject *parent = nullptr);

    ModuleObject *module() override;

    QString name() const override;

    QString location() const override;
};

#endif // SYNCPLUGIN_H
