#include "player_ui_manager.h"

#include <QDebug>
#include <QString>
#include <QQmlContext>
#include <QQmlEngine>
#include "player.h"

PlayerUIManager::PlayerUIManager(QObject *parent)
    : QObject(parent)
    , m_updateDataModel(new PlayerUiData())
    , m_updateInfoViewModel(new PlayerUiViewFwInfo(m_updateDataModel, parent))
    , m_downloadViewModel(new PlayerUiViewFwDownload(m_updateDataModel, parent))
    , m_installViewModel(new PlayerUiViewFwInstall(m_updateDataModel, parent))
{
}

void PlayerUIManager::registerMetaTypes()
{
    qmlRegisterUncreatableType<PlayerUIManager>("Player", 1, 0, "PlayerUIManager", "Should only be used as a context property");
    qApp->qmlApplicationEngine()->rootContext()->setContextProperty("swUpdateManager", this);

    qmlRegisterUncreatableType<PlayerUiViewFwInfo>("Player", 1, 0, "PlayerUiViewFwInfo", "Should only be used as a context property");
    qApp->qmlApplicationEngine()->rootContext()->setContextProperty("swUpdateInfoViewModel", m_updateInfoViewModel);

    qmlRegisterUncreatableType<PlayerUiViewFwDownload>("Player", 1, 0, "PlayerUiViewFwDownload", "Should only be used as a context property");
    qApp->qmlApplicationEngine()->rootContext()->setContextProperty("swUpdateDownloadViewModel", m_downloadViewModel);

    qmlRegisterUncreatableType<PlayerUiViewFwInstall>("Player", 1, 0, "PlayerUiViewFwInstall", "Should only be used as a context property");
    qApp->qmlApplicationEngine()->rootContext()->setContextProperty("swUpdateInstallViewModel", m_installViewModel);
}

void PlayerUIManager::checkForUpdates()
{
    m_updateDataModel->checkForSwUpdate();
}

void PlayerUIManager::downloadUpdate()
{
    m_updateDataModel->performDownload();
}

void PlayerUIManager::installUpdate()
{
    m_updateDataModel->performInstallation();
}

void PlayerUIManager::updateError(QString ErrorMessage)
{
    m_updateDataModel->updateError(ErrorMessage);
}
