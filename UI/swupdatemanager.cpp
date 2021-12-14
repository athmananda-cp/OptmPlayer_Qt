#include "swupdatemanager.h"

#include <QDebug>
#include <QString>
#include <QQmlContext>
#include <QQmlEngine>
#include "player.h"

SwUpdateManager::SwUpdateManager(QObject *parent)
    : QObject(parent)
    , m_updateDataModel(new SwUpdateDataModel())
    , m_updateInfoViewModel(new SwUpdateInfoViewModel(m_updateDataModel, parent))
    , m_downloadViewModel(new SwUpdateDownloadViewModel(m_updateDataModel, parent))
    , m_installViewModel(new SwUpdateInstallViewModel(m_updateDataModel, parent))
{
}

void SwUpdateManager::registerMetaTypes()
{
    qmlRegisterUncreatableType<SwUpdateManager>("Player", 1, 0, "SwUpdateManager", "Should only be used as a context property");
    qApp->qmlApplicationEngine()->rootContext()->setContextProperty("swUpdateManager", this);

    qmlRegisterUncreatableType<SwUpdateInfoViewModel>("Player", 1, 0, "SwUpdateInfoViewModel", "Should only be used as a context property");
    qApp->qmlApplicationEngine()->rootContext()->setContextProperty("swUpdateInfoViewModel", m_updateInfoViewModel);

    qmlRegisterUncreatableType<SwUpdateDownloadViewModel>("Player", 1, 0, "SwUpdateDownloadViewModel", "Should only be used as a context property");
    qApp->qmlApplicationEngine()->rootContext()->setContextProperty("swUpdateDownloadViewModel", m_downloadViewModel);

    qmlRegisterUncreatableType<SwUpdateInstallViewModel>("Player", 1, 0, "SwUpdateInstallViewModel", "Should only be used as a context property");
    qApp->qmlApplicationEngine()->rootContext()->setContextProperty("swUpdateInstallViewModel", m_installViewModel);
}

void SwUpdateManager::checkForUpdates()
{
    m_updateDataModel->checkForSwUpdate();
}

void SwUpdateManager::downloadUpdate()
{
    m_updateDataModel->performDownload();
}

void SwUpdateManager::installUpdate()
{
    m_updateDataModel->performInstallation();
}
