#include "swupdatedatamodel.h"
#include "utils.h"

#include <QDebug>

SwUpdateDataModel::SwUpdateDataModel(QObject *parent) : QObject(parent)
{
    connect(&m_deviceConnect, &DeviceConnect::swVersionInfoAvailable, this, &SwUpdateDataModel::onSwVersionInfoAvailable);

    connect(&m_deviceConnect, &DeviceConnect::binaryDownloadStarted, this, &SwUpdateDataModel::binaryDownloadStarted);
    connect(&m_deviceConnect, &DeviceConnect::downloadProgressChanged, this, &SwUpdateDataModel::downloadProgressChanged);
    connect(&m_deviceConnect, &DeviceConnect::binaryDownloadCompleted, this, &SwUpdateDataModel::binaryDownloadCompleted);

    connect(&m_deviceConnect, &DeviceConnect::binaryInstallationStarted, this, &SwUpdateDataModel::swUpdateInstallationStarted);
    connect(&m_deviceConnect, &DeviceConnect::installationProgressChanged, this, &SwUpdateDataModel::swUpdateInstallationProgressChanged);
    connect(&m_deviceConnect, &DeviceConnect::binaryInstallationCompleted, this, &SwUpdateDataModel::swUpdateInstallationCompleted);
}

SwUpdateDataModel::~SwUpdateDataModel()
{

}

void SwUpdateDataModel::checkForSwUpdate()
{
    m_deviceConnect.requestSwVersionInfo();
}

void SwUpdateDataModel::performDownload()
{
    m_deviceConnect.downloadUpdateBinaries();
}

void SwUpdateDataModel::performInstallation()
{
    m_deviceConnect.installUpdatedBinaries();
}

void SwUpdateDataModel::onSwVersionInfoAvailable(const SwUpdateInfo_t &swUpdateInfo)
{
    m_swUpdateInfo = swUpdateInfo;

    // Verify if SW update is needed
    VersionInfo_t versionInfo = readCurrentVersionInfo();
    if (! versionInfo.equals(swUpdateInfo._VersionInfo))
        emit swUpdateAvaialble(swUpdateInfo);
    else
        qDebug() << "Software upto date. No update required";
}

const SwUpdateInfo_t &SwUpdateDataModel::swUpdateInfo() const
{
    return m_swUpdateInfo;
}

