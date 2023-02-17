#include "syncplugin.h"
#include <QTranslator>
#include <QCoreApplication>
#include <QLocale>
#include "syncmodule.h"

SyncPlugin::SyncPlugin(QObject *parent):PluginInterface(parent)
{
    // 插件加载翻译
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/dcc-deepinid-plugin/translations/dcc-deepinid-plugin_%1.qm").arg(QLocale::system().name()));

    QCoreApplication::installTranslator(translator);
}

ModuleObject *SyncPlugin::module()
{
    return new SyncModule();
}

QString SyncPlugin::name() const
{
    return QStringLiteral("cloudsync");
}

QString SyncPlugin::location() const
{
    return QStringLiteral("1");
}
