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
    connect(m_updateDataModel.data(), &SwUpdateDataModel::swUpdateAvaialble, this, &SwUpdateManager::onSwUpdateAvaialble);

    connect(m_updateDataModel.data(), &SwUpdateDataModel::binaryDownloadStarted, this, &SwUpdateManager::swUpdatedDownloadStarted);
    connect(m_updateDataModel.data(), &SwUpdateDataModel::binaryDownloadCompleted, this, [=]() {
        installUpdate();
        emit swUpdateDownloadCompleted();
    });

    connect(m_updateDataModel.data(), &SwUpdateDataModel::swUpdateInstallationStarted, this, &SwUpdateManager::swUpdateInstallationStarted);
    connect(m_updateDataModel.data(), &SwUpdateDataModel::swUpdateInstallationCompleted, this, &SwUpdateManager::swUpdateInstallationCompleted);
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
    emit swUpdateCheckStarted();
    m_updateDataModel->checkForSwUpdate();
    emit swUpdateCheckCompleted();
}

void SwUpdateManager::downloadUpdate()
{
    m_updateDataModel->performDownload();
}

void SwUpdateManager::installUpdate()
{
    m_updateDataModel->performInstallation();
}

void SwUpdateManager::onSwUpdateAvaialble(const SwUpdateInfo_t &swUpdateInfo)
{
    m_updateInfoViewModel->setSwUpdateInfo(swUpdateInfo);
    emit swUpdateAvailable();
}
