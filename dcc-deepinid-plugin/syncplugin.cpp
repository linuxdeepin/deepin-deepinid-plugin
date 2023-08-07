#include "syncplugin.h"
#include <QTranslator>
#include <QCoreApplication>
#include <QLocale>
#include "syncmodule.h"

SyncPlugin::SyncPlugin(QObject *parent):PluginInterface(parent)
{
}

ModuleObject *SyncPlugin::module()
{
    return new SyncModule();
}

QString SyncPlugin::name() const
{
    return QStringLiteral("dcc-deepinid-plugin");
}

QString SyncPlugin::location() const
{
    return QStringLiteral("0");
}
