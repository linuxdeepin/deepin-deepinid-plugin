#include "securitymanage.h"
#include <DLabel>
#include <DTipLabel>
#include <QVBoxLayout>
#include <DListView>
#include <DDialog>
#include <DPasswordEdit>
#include <DSuggestButton>
#include <DFontSizeManager>
#include <DFrame>
#include <QTimer>
#include <DGuiApplicationHelper>
#include "utils.h"
#include "trans_string.h"

DWIDGET_USE_NAMESPACE
Q_DECLARE_METATYPE(QMargins)

static const QString STRING_ICONUOSID = QStringLiteral("dcc_union_id");

SecurityPage::SecurityPage(QWidget *parent) : QWidget(parent)
  , m_phoneList(new DListView(this))
  , m_accountList(new DListView(this))
  , m_passwdList(new DListView(this))
  , m_phoneModel(new QStandardItemModel(this))
  , m_accountModel(new QStandardItemModel(this))
  , m_passwdModel(new QStandardItemModel(this))
  , m_unbindWeChatDlg(new DDialog("", "", this))
{
    m_forgetUrl = utils::forgetPwdURL();
    //qDebug() << "forget url:" << m_forgetUrl;
    m_wechatUrl = utils::wechatURL();
    //qDebug() << "wechat url:" << m_wechatUrl;

    initUI();
    initDialog();
    initConnection();
    initModelData();
}

void SecurityPage::setSyncModel(SyncModel *syncModel)
{
    m_syncModel = syncModel;
    connect(m_syncModel, &SyncModel::userInfoChanged, this, &SecurityPage::onUserInfoChanged);
}

void SecurityPage::setSyncWorker(SyncWorker *worker)
{
    m_syncWorker = worker;
}

void SecurityPage::onLogin()
{
    //init data
    onUserInfoChanged(m_syncModel->userinfo());
}

void SecurityPage::initUI()
{
    QVBoxLayout *mainlayout = new QVBoxLayout;
    //head
    QVBoxLayout *headlayout = new QVBoxLayout;
    headlayout->setContentsMargins(0, 5, 0, 0);
    headlayout->setSpacing(10);
    DLabel *phone = new DLabel(TransString::getTransString(STRING_TITLEPHONE), this);
    DTipLabel *phoneTip = new DTipLabel(TransString::getTransString(STRING_MSGPHONE), this);
    m_phoneList->setBackgroundType(DStyledItemDelegate::BackgroundType::ClipCornerBackground);
    m_phoneList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_phoneList->setSelectionMode(QListView::SelectionMode::NoSelection);
    m_phoneList->setEditTriggers(DListView::NoEditTriggers);
    m_phoneList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_phoneList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_phoneList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_phoneList->setFrameShape(DListView::NoFrame);
    m_phoneList->setItemSpacing(1);
    m_phoneList->setViewportMargins(0, 0, 1, 0);
    m_phoneList->setModel(m_phoneModel);
    m_phoneList->setIconSize(QSize(16, 16));

    phoneTip->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    phoneTip->setWordWrap(true);
    QHBoxLayout *tiplayout = new QHBoxLayout();
    tiplayout->setContentsMargins(0, 0, 0, 0);
    tiplayout->addWidget(phoneTip);
    phone->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    phone->setWordWrap(true);
    QHBoxLayout *phoneLayout = new QHBoxLayout;
    phoneLayout->setContentsMargins(0, 0, 0, 0);
    phoneLayout->addWidget(phone);
    headlayout->addLayout(phoneLayout);
    headlayout->addSpacing(-5);
    headlayout->addLayout(tiplayout);
    headlayout->addWidget(m_phoneList);

    //bind
    QVBoxLayout *bindlayout = new QVBoxLayout;
    bindlayout->setSpacing(10);
    DLabel *bindThird = new DLabel(TransString::getTransString(STRING_WECHATTITLE), this);
    DTipLabel *bindThirdTip = new DTipLabel(TransString::getTransString(STRING_WECHATMSG), this);
    bindThirdTip->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    bindThirdTip->setWordWrap(true);
    m_accountList->setBackgroundType(DStyledItemDelegate::BackgroundType::ClipCornerBackground);
    m_accountList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_accountList->setSelectionMode(QListView::SelectionMode::NoSelection);
    m_accountList->setEditTriggers(DListView::NoEditTriggers);
    m_accountList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_accountList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_accountList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_accountList->setFrameShape(DListView::NoFrame);
    m_accountList->setItemSpacing(0);
    m_accountList->setViewportMargins(0, 0, 1, 0);
    m_accountList->setModel(m_accountModel);
    m_accountList->setIconSize(QSize(16, 16));
    QHBoxLayout *acctipLayout = new QHBoxLayout;
    acctipLayout->setContentsMargins(0, 0, 0, 0);
    acctipLayout->addWidget(bindThirdTip);
    bindlayout->addWidget(bindThird, 0, Qt::AlignLeft);
    bindlayout->addSpacing(-5);
    bindlayout->addLayout(acctipLayout);
    bindlayout->addWidget(m_accountList);

    //password
    QVBoxLayout *passwdlayout = new QVBoxLayout;
    passwdlayout->setSpacing(10);
    DLabel *passwd = new DLabel(TransString::getTransString(STRING_PASSWORDTITLE), this);
    DTipLabel *passwdTip = new DTipLabel(TransString::getTransString(STRING_PASSWORDMSG), this);
    m_passwdList->setBackgroundType(DStyledItemDelegate::BackgroundType::ClipCornerBackground);
    m_passwdList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_passwdList->setSelectionMode(QListView::SelectionMode::NoSelection);
    m_passwdList->setEditTriggers(DListView::NoEditTriggers);
    m_passwdList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_passwdList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_passwdList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_passwdList->setFrameShape(DListView::NoFrame);
    m_passwdList->setItemSpacing(10);
    m_passwdList->setViewportMargins(0, 0, 1, 0);
    m_passwdList->setModel(m_passwdModel);
    m_passwdList->setIconSize(QSize(16, 16));
    passwdlayout->addWidget(passwd, 0, Qt::AlignLeft);
    passwdlayout->addSpacing(-5);
    passwdlayout->addWidget(passwdTip, 0, Qt::AlignLeft);
    passwdlayout->addWidget(m_passwdList);
    //
    DFontSizeManager::instance()->bind(phone, DFontSizeManager::T5, QFont::DemiBold);
    DFontSizeManager::instance()->bind(phoneTip, DFontSizeManager::T8, QFont::Thin);
    DFontSizeManager::instance()->bind(bindThird, DFontSizeManager::T5, QFont::DemiBold);
    DFontSizeManager::instance()->bind(bindThirdTip, DFontSizeManager::T8, QFont::Thin);
    DFontSizeManager::instance()->bind(passwd, DFontSizeManager::T5, QFont::DemiBold);
    DFontSizeManager::instance()->bind(passwdTip, DFontSizeManager::T8, QFont::Thin);

    mainlayout->setMargin(20);
    mainlayout->setSpacing(20);
    mainlayout->addLayout(headlayout);
    mainlayout->addLayout(bindlayout);
    mainlayout->addLayout(passwdlayout);
    mainlayout->addStretch();
    setLayout(mainlayout);
}

void SecurityPage::initConnection()
{
    connect(m_unbindWeChatDlg, &QDialog::accepted, this, [this]{
        this->verifyPasswd(UnbindAccountType);
    });
    connect(this, &SecurityPage::onUserLogout, m_unbindWeChatDlg, &QDialog::reject);
}

void SecurityPage::initModelData()
{
    QMargins itemMargin(10, 6, 10, 6);
    DFontSizeManager::SizeType actionFontSize = DFontSizeManager::T7;
    DStandardItem *itemPhone = new DStandardItem;
    itemPhone->setIcon(QIcon::fromTheme("dcc_trust_phone"));
    itemPhone->setData(QVariant::fromValue(itemMargin), Dtk::MarginsRole);
    itemPhone->setSizeHint(QSize(-1, 37));
    DViewItemAction *phoneAction = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
    phoneAction->setText("");
    phoneAction->setFontSize(actionFontSize);
    phoneAction->setTextColorRole(DPalette::Link);
    itemPhone->setActionList(Qt::RightEdge, {phoneAction});
    connect(phoneAction, &QAction::triggered, [=]{
        this->verifyPasswd(PhoneType);
    });
    m_phoneModel->appendRow(itemPhone);

    DStandardItem *itemMail = new DStandardItem;
    itemMail->setIcon(QIcon::fromTheme("dcc_trust_email"));
    itemMail->setData(QVariant::fromValue(itemMargin), Dtk::MarginsRole);
    itemMail->setSizeHint(QSize(-1, 37));
    DViewItemAction *mailAction = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
    mailAction->setText("");
    mailAction->setFontSize(actionFontSize);
    mailAction->setTextColorRole(DPalette::Link);
    itemMail->setActionList(Qt::RightEdge, {mailAction});
    connect(mailAction, &QAction::triggered, [=]{
        this->verifyPasswd(MailType);
    });
    m_phoneModel->appendRow(itemMail);

    DStandardItem *itemAccount = new DStandardItem;
    itemAccount->setIcon(QIcon::fromTheme("dcc_secwechat"));
    itemAccount->setData(QVariant::fromValue(itemMargin), Dtk::MarginsRole);
    itemAccount->setSizeHint(QSize(-1, 36));
    DViewItemAction *accountAction = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
    accountAction->setText("");
    accountAction->setFontSize(actionFontSize);
    accountAction->setTextColorRole(DPalette::Link);
    itemAccount->setActionList(Qt::RightEdge, {accountAction});
    m_accountModel->appendRow(itemAccount);
    connect(accountAction, &QAction::triggered, [=]{
        auto &infos = m_syncModel->userinfo();
        QString wechatNick = infos["WechatNickname"].toString();
        if(wechatNick.isEmpty()) {
            this->verifyPasswd(BindAccountType);
        }
        else {
            m_unbindWeChatDlg->exec();
        }
    });

    DStandardItem *itemPasswd = new DStandardItem;
    itemPasswd->setIcon(QIcon::fromTheme("dcc_seckey"));
    itemPasswd->setData(QVariant::fromValue(itemMargin), Dtk::MarginsRole);
    itemPasswd->setText(TransString::getTransString(STRING_PASSWORDTITLE));
    itemPasswd->setSizeHint(QSize(-1, 46));
    DViewItemAction *passwdAction = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
    passwdAction->setText(TransString::getTransString(STRING_PWDACTION));
    passwdAction->setFontSize(actionFontSize);
    passwdAction->setTextColorRole(DPalette::Link);
    itemPasswd->setActionList(Qt::RightEdge, {passwdAction});
    m_passwdModel->appendRow(itemPasswd);
    connect(passwdAction, &QAction::triggered, [=]{
        this->verifyPasswd(PasswdType);
    });
}

void SecurityPage::initDialog()
{
    m_unbindWeChatDlg->setFixedWidth(400);
    //m_unbindWeChatDlg->setMinimumHeight(226);
    m_unbindWeChatDlg->setIcon(QIcon::fromTheme(STRING_ICONUOSID));
    m_unbindWeChatDlg->setMessage(TransString::getTransString(STRING_WECHATUNBIND));
    m_unbindWeChatDlg->addButton(TransString::getTransString(STRING_CANCEL));
    m_unbindWeChatDlg->addButton(TransString::getTransString(STRING_CONFIRM), true, DDialog::ButtonWarning);
}

void SecurityPage::initRegisterDialog(RegisterDlg *dlg)
{
    connect(dlg, &RegisterDlg::registerPasswd, this, [=](const QString &strpwd){
        m_syncWorker->registerPasswd(strpwd);
        dlg->accept();
    });
    connect(this, &SecurityPage::onUserLogout, dlg, &QDialog::reject);
}

void SecurityPage::initVerifyDialog(VerifyDlg *dlg)
{
    connect(dlg, &VerifyDlg::verifyPasswd, [=](const QString &strpwd){
        QString encryptPwd;
        int remainNum = 0;
        if(m_syncWorker->checkPassword(strpwd, encryptPwd, remainNum))
        {
            qInfo() << __LINE__ << "check password success";
            m_lastEnPwd = encryptPwd;
            dlg->accept();
        }
        else
        {
            qInfo() << __LINE__ << "check password failed:";
            //tip passwd is wrong
            dlg->showAlert(utils::getRemainPasswdMsg(remainNum));
        }
    });
    connect(dlg, &VerifyDlg::forgetPasswd, [this]{
        m_syncWorker->openForgetPasswd(QString("%1&time=%2").arg(m_forgetUrl).arg(QDateTime::currentMSecsSinceEpoch()));
    });
    connect(this, &SecurityPage::onUserLogout, dlg, &QDialog::reject);
}

void SecurityPage::initPhoneDialog(PhoneMailDlg *dlg)
{
    qInfo() << "init phone dialog";
    dlg->setTitle(TransString::getTransString(STRING_PHONEBIND));
    dlg->initPhoneNumConstraint();
    dlg->setNumHolderText(TransString::getTransString(STRING_PHONEHOLDER));
    dlg->setInvalidTip(TransString::getTransString(STRING_PHONEINVALID));
    initPhoneMailConnection(dlg);
}

void SecurityPage::initMailDialog(PhoneMailDlg *dlg)
{
    qInfo() << "init mail dialog";
    dlg->setTitle(TransString::getTransString(STRING_MAILBIND));
    dlg->initMailNumConstraint();
    dlg->setNumHolderText(TransString::getTransString(STRING_MAILHOLDER));
    dlg->setInvalidTip(TransString::getTransString(STRING_MAILINVALID));
    initPhoneMailConnection(dlg);
}

void SecurityPage::initPhoneMailConnection(PhoneMailDlg *dlg)
{
    connect(dlg, &PhoneMailDlg::sendVerify, [=](const QString &strNum){
        int countDown = m_syncWorker->sendVerifyCode(strNum);
        if(countDown > 0) {
            dlg->verifyCoolDown(countDown);
        }
        else {
            dlg->showVerifyAlert();
        }
    });
    connect(dlg, &PhoneMailDlg::updatePhoneMail, [=](const QString &strnum, const QString &verify){
        QString rebindAccount;
        QString rebindKey;
        //qDebug() << "update phonemail:" << strnum << verify;
        bool ret = m_syncWorker->updatePhoneEmail(strnum, verify, rebindAccount, rebindKey);
        if(!ret)
        {
            if(rebindAccount.isEmpty()) {
                qDebug() << "show fail";
                utils::sendSysNotify(TransString::getTransString(STRING_FAILTIP));
            }
            else {
                qDebug() << "show rebind dialog:" << rebindAccount << rebindKey;
                dlg->setRebindInfo(rebindAccount, rebindKey);
            }
        }
        else
        {
            utils::sendSysNotify(TransString::getTransString(STRING_SUCCESSTIP));
        }

        dlg->accept();
    });
    connect(dlg, &PhoneMailDlg::rebindPhoneMail, [=](const QString &strnum, const QString &verify, const QString &key){
        qInfo() << strnum << verify << key;
        QString rebindAccount;
        QString rebindKey = key;
        bool ret = m_syncWorker->updatePhoneEmail(strnum, verify, rebindAccount, rebindKey);
        if(!ret)
        {
            utils::sendSysNotify(TransString::getTransString(STRING_FAILTIP));
        }
        else
        {
            utils::sendSysNotify(TransString::getTransString(STRING_SUCCESSTIP));
        }
    });
    connect(this, &SecurityPage::onUserLogout, dlg, &QDialog::reject);
}

void SecurityPage::initWeChatDialog(WeChatDlg *dlg)
{
    connect(dlg, &WeChatDlg::bindSuccess, [this]{
        utils::sendSysNotify(TransString::getTransString(STRING_SUCCESSTIP));
        this->m_syncWorker->refreshUserInfo();
    });
    connect(this, &SecurityPage::onUserLogout, dlg, &QDialog::reject);

    QString bindurl;
    QString strsession = m_syncWorker->getSessionID();
    if(strsession.isEmpty()) {
        bindurl = "qrc:/web/error.html";
    }
    else {
        bindurl = m_wechatUrl;
        bindurl += "&sessionid=";
        bindurl += strsession;
        bindurl += QString("&time=%1").arg(QDateTime::currentMSecsSinceEpoch());
    }

    qDebug() << "set bind url:" << bindurl;
    dlg->setPageUrl(bindurl);
}

void SecurityPage::initResetPwdDialog(ResetPwdDlg *dlg)
{
    connect(dlg, &ResetPwdDlg::resetPasswd, this, [=](const QString &newpwd){
        if(m_syncWorker->resetPassword(m_lastEnPwd, newpwd)) {
            utils::sendSysNotify(TransString::getTransString(STRING_RESETPWDTIP));
            dlg->accept();
        }
        else {
            dlg->showFailTip();
        }
    });
    connect(this, &SecurityPage::onUserLogout, dlg, &QDialog::reject);
}

void SecurityPage::onUserInfoChanged(const QVariantMap &infos)
{
    QString phone = infos["Phone"].toString();
    QString mail = infos["Email"].toString();
    QString wechat = infos["WechatNickname"].toString();
    DStandardItem *phoneItem = dynamic_cast<DStandardItem*>(m_phoneModel->item(0));
    DStandardItem *mainItem = dynamic_cast<DStandardItem*>(m_phoneModel->item(1));
    phoneItem->setText(phone.trimmed().isEmpty() ? TransString::getTransString(STRING_UNLINKED) : phone.trimmed());
    phoneItem->actionList(Qt::RightEdge).at(0)->setText(phone.isEmpty() ? TransString::getTransString(STRING_BINDTIP) : TransString::getTransString(STRING_MODIFY));
    mainItem->setText(mail.trimmed().isEmpty() ? TransString::getTransString(STRING_UNLINKED) : mail.trimmed());
    mainItem->actionList(Qt::RightEdge).at(0)->setText(mail.isEmpty() ? TransString::getTransString(STRING_BINDTIP): TransString::getTransString(STRING_MODIFY));

    DStandardItem *wechatItem = dynamic_cast<DStandardItem*>(m_accountModel->item(0));
    wechatItem->setText(wechat.trimmed().isEmpty() ? TransString::getTransString(STRING_UNLINKED) : wechat.trimmed());
    DViewItemAction *wechatAction = wechatItem->actionList(Qt::RightEdge).at(0);
    wechatAction->setText(wechat.isEmpty() ? TransString::getTransString(STRING_BINDTIP) : TransString::getTransString(STRING_UNBINDTIP));
}

bool SecurityPage::verifyPasswd(VerifyType type)
{
    int ret;
    //check password is empty
    bool isEmpty = false;
    if(!m_syncWorker->checkPasswdEmpty(isEmpty))
    {
        qWarning() << "check password empty failed";
        return false;
    }

    if(isEmpty)
    {
        //password is empty
        RegisterDlg regDlg;
        initRegisterDialog(&regDlg);
        ret = regDlg.exec();
    }
    else
    {
        VerifyDlg verifyDlg;
        initVerifyDialog(&verifyDlg);
        ret = verifyDlg.exec();
    }

    qDebug() << __LINE__ << type << ", verify pwd ret:" << (ret == QDialog::Accepted);
    if(ret == QDialog::Accepted)
    {
        if(type != PasswdType || !isEmpty)
        {
            openUserDialog(type);
        }
    }

    return true;
}

void SecurityPage::openUserDialog(SecurityPage::VerifyType type)
{
    switch (type)
    {
    case PhoneType:
    {
        bool bFinish = true;
        do
        {
            PhoneMailDlg phoneDlg;
            initPhoneDialog(&phoneDlg);
            phoneDlg.exec();
            qInfo() << "phone update done";
            if(!phoneDlg.rebindAccount().isEmpty())
            {
                ReBindDlg rebindDlg;
                rebindDlg.setTitleTip(TransString::getTransString(STRING_PHONEBIND));
                rebindDlg.setChangeTip(TransString::getTransString(STRING_USEANOPHONE));
                rebindDlg.setDialogMessage(TransString::getTransString(STRING_REBINDPHONE).arg(phoneDlg.rebindAccount()));
                connect(this, &SecurityPage::onUserLogout, &rebindDlg, &QDialog::reject);
                rebindDlg.exec();
                if(rebindDlg.result() == QDialog::Accepted)
                {
                    if(rebindDlg.isChangeNum()) {
                        bFinish = false;
                        continue;
                    }
                    else {
                        phoneDlg.bindToLocal();
                    }
                }
            }

            bFinish = true;
        }while (!bFinish);
    }
        break;
    case MailType:
    {
        bool bFinish = true;
        do
        {
            PhoneMailDlg mailDlg;
            initMailDialog(&mailDlg);
            mailDlg.exec();
            qInfo() << "mail update done";
            if(!mailDlg.rebindAccount().isEmpty())
            {
                ReBindDlg rebindDlg;
                rebindDlg.setTitleTip(TransString::getTransString(STRING_MAILBIND));
                rebindDlg.setChangeTip(TransString::getTransString(STRING_USEANOMAIL));
                rebindDlg.setDialogMessage(TransString::getTransString(STRING_REBINDMAIL).arg(mailDlg.rebindAccount()));
                connect(this, &SecurityPage::onUserLogout, &rebindDlg, &QDialog::reject);
                rebindDlg.exec();
                if(rebindDlg.result() == QDialog::Accepted)
                {
                    if(rebindDlg.isChangeNum()) {
                        bFinish = false;
                        continue;
                    }
                    else {
                        mailDlg.bindToLocal();
                    }
                }
            }

            bFinish = true;
        }while (!bFinish);
    }
        break;
    case BindAccountType:
    {
        WeChatDlg wechatDlg;
        initWeChatDialog(&wechatDlg);
        wechatDlg.exec();
    }
        break;
    case UnbindAccountType:
    {
        m_syncWorker->unBindPlatform();
        utils::sendSysNotify(TransString::getTransString(STRING_SUCCESSTIP));
    }
        break;
    case PasswdType:
    {
        ResetPwdDlg resetDlg;
        initResetPwdDialog(&resetDlg);
        resetDlg.exec();
    }
        break;
    }
}
