#include "interface/frameproxyinterface.h"

#include "syncmodule.h"
#include "syncworker.h"
#include "syncmodel.h"
#include "pages/logininfodetail.h"
#include "utils.h"
#include "syncwidget.h"

#include <DSysInfo>

using namespace DCC_NAMESPACE;
using namespace DCC_NAMESPACE::sync;
using namespace dcc::cloudsync;

SyncModule::SyncModule(FrameProxyInterface *frameProxy, QObject *parent)
    : QObject(parent)
    , ModuleInterface(frameProxy)
    , m_model(nullptr)
    , m_worker(nullptr)
{
    // 插件加载翻译
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/dcc-deepinid-plugin/translations/dcc-deepinid-plugin_%1.qm").arg(QLocale::system().name()));
    QCoreApplication::installTranslator(translator);
}

SyncModule::~SyncModule()
{

}

void SyncModule::initialize()
{

}

const QString SyncModule::name() const
{
    return QStringLiteral("cloudsync");
}

const QString SyncModule::displayName() const
{
    return DSysInfo::isCommunityEdition() ? tr("Deepin ID") : tr("Union ID");
}

void SyncModule::contentPopped(QWidget *const w)
{
    Q_UNUSED(w);
}

void SyncModule::active()
{
    SyncWidget *widget = new SyncWidget;
    widget->setVisible(false);

    connect(widget, &SyncWidget::requestLoginUser, m_worker, &dcc::cloudsync::SyncWorker::loginUser, Qt::UniqueConnection);

    connect(widget, &SyncWidget::requestAsyncLogoutUser, m_worker, &dcc::cloudsync::SyncWorker::logoutUser, Qt::QueuedConnection);
    connect(widget, &SyncWidget::requestSetFullname, m_worker, &dcc::cloudsync::SyncWorker::onSetFullname);

    connect(widget, &SyncWidget::requestBindAccount, m_worker, &dcc::cloudsync::SyncWorker::asyncBindAccount, Qt::UniqueConnection);
    connect(widget, &SyncWidget::requestUnBindAccount, m_worker, &dcc::cloudsync::SyncWorker::asyncUnbindAccount, Qt::UniqueConnection);
    connect(widget, &SyncWidget::requestSetAutoSync, m_worker, &dcc::cloudsync::SyncWorker::setAutoSync, Qt::UniqueConnection);
    connect(widget, &SyncWidget::requestLocalBindCheck, m_worker, &dcc::cloudsync::SyncWorker::asyncLocalBindCheck, Qt::UniqueConnection);
    connect(widget, &SyncWidget::requestSetModuleState, m_worker, &dcc::cloudsync::SyncWorker::setSyncState, Qt::QueuedConnection);

    connect(widget, &SyncWidget::requestUOSID, m_worker, &dcc::cloudsync::SyncWorker::getUOSID, Qt::UniqueConnection);
    connect(widget, &SyncWidget::requestUUID, m_worker, &dcc::cloudsync::SyncWorker::getUUID, Qt::UniqueConnection);
    connect(widget, &SyncWidget::requestHostName, m_worker, &dcc::cloudsync::SyncWorker::getHostName, Qt::UniqueConnection);

    m_frameProxy->pushWidget(this, widget);
    widget->setModel(m_model);
    widget->setVisible(true);

    m_worker->activate(); //refresh data
}

void SyncModule::preInitialize(bool sync, FrameProxyInterface::PushType pushtype)
{
    if (!DSysInfo::isDeepin()) {
        qInfo() << "module: " << displayName() << " is disable now!";
        m_frameProxy->setModuleVisible(this, false);
        setDeviceUnavailabel(true);
        return;
    }

    Q_UNUSED(sync);
    Q_UNUSED(pushtype);
    m_model = new SyncModel;
    m_worker = new SyncWorker(m_model);

    bool visible = !IsServerSystem && m_model->syncIsValid();
    m_frameProxy->setModuleVisible(this, visible);
    setDeviceUnavailabel(!visible);
    connect(m_model, &SyncModel::syncIsValidChanged, this, [=](bool valid) {
        bool visible = valid && !IsServerSystem;
        m_frameProxy->setModuleVisible(this, visible);
        setDeviceUnavailabel(!visible);
    });

    addChildPageTrans();
    initSearchData();
}

QStringList SyncModule::availPage() const
{
    QStringList sl;
    sl << "Cloud Sync" << "Sync ID Sign In";
    return sl;
}

void SyncModule::addChildPageTrans() const
{
    if (m_frameProxy != nullptr) {
        m_frameProxy->addChildPageTrans("Sign In", tr("Sign In"));
    }
}

QString SyncModule::translationPath() const
{
    return QString(":/translations/dcc-deepinid-plugin_%1.ts");
}

QString SyncModule::path() const
{
    return QStringLiteral("mainwindow");
}

QString SyncModule::follow() const
{
    return QStringLiteral("display");
}

void SyncModule::initSearchData()
{
    auto tfunc = [this]() {
        m_frameProxy->setWidgetVisible(displayName(), tr("Sign In"), true);
    };

    tfunc();
}
