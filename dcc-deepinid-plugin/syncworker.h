#ifndef SYNCWORKER_H
#define SYNCWORKER_H

#include "syncmodel.h"

#include <QObject>
#include <com_deepin_sync_daemon.h>
#include <com_deepin_deepinid.h>

using SyncInter = com::deepin::sync::Daemon;
using DeepinId = com::deepin::deepinid;

struct BindCheckResult {
    QString ubid = "";
    QString error = "";
    bool ret = false;
};

class SyncWorker : public QObject
{
    Q_OBJECT
public:
    explicit SyncWorker(SyncModel * model, QObject *parent = nullptr);

    void activate();
    void deactivate();
    void refreshSyncState();

public Q_SLOTS:
    void setSync(std::pair<SyncType, bool> state);
    void setSyncState(const QString &syncType, bool state);
    void loginUser();
    void logoutUser();
    void asyncLogoutUser(const QString &ubid);
    void setAutoSync(bool autoSync);
    void licenseStateChangeSlot();

    void getUOSID();
    void getUUID();
    void getHostName();

    void asyncLocalBindCheck(const QString &uuid);
    void asyncBindAccount(const QString &uuid, const QString &hostName);
    void asyncUnbindAccount(const QString &ubid);

    void onSetFullname(const QString &fullname);
    /**
     * @brief onPullMessage  插件打开 若是登录状态 用户停留5秒以上 进行同步操作
     */
    void onPullMessage();

private:
    void getUserDeepinidInfo();

private:
    void onSyncModuleStateChanged(const QString& module, bool enable);
    void onStateChanged(const IntString& state);
    void onLastSyncTimeChanged(qlonglong lastSyncTime);
    void getLicenseState();
    BindCheckResult checkLocalBind(const QString &uuid);
    BindCheckResult logout(const QString &uuid);
    BindCheckResult bindAccount(const QString &uuid, const QString &hostName);
    BindCheckResult unBindAccount(const QString &ubid);

private:
    SyncModel *m_model;
    SyncInter *m_syncInter;
    DeepinId *m_deepinId_inter;
    QDBusInterface *m_syncHelperInter;
};

#endif // SYNCWORKER_H
