#include "deviceconnect.h"
#include "player.h"
#include "network/networkmanager.h"

#include <QTimer>

DeviceConnect::DeviceConnect(QObject *parent) : QObject(parent)
{

}

void DeviceConnect::requestSwVersionInfo()
{
    // Connect to the server for receiving the sw version info
    connect(qApp->networkManager().data(), &NetworkManager::upgradeInfoAvailable, [this](const UpgradeInfo_t upgardeInfo) {
        SwUpdateInfo_t swUpdateInfo;
        swUpdateInfo._VersionInfo._MajorNumber = upgardeInfo.MajorVersion;
        swUpdateInfo._VersionInfo._MinorNumber = upgardeInfo.MinorVersion;
        swUpdateInfo._VersionInfo._MaintenanceNumber = 0;
        swUpdateInfo._Enhancements = upgardeInfo.ListOfChanges.join("");

        emit swVersionInfoAvailable(swUpdateInfo);
    });
    qApp->networkManager()->getUpgradeJson();
}

void DeviceConnect::downloadUpdateBinaries()
{
    // TODO: Connect to the device for receiving the latest binary package
    emit binaryDownloadStarted();

    //--> TODO : Temp Code.
    // For the demonstration purpose, instead of downloading the binary
    // it would be picked from the device itself. To simulate the download
    // below timer emits the face download info
//    float downloadedSize = 0;
//    QTimer *timer = new QTimer();
//    timer->setInterval(100);
//    connect(timer, &QTimer::timeout, [this, timer, downloadedSize]() mutable {

//        emit downloadProgressChanged(downloadedSize);
//        if (downloadedSize >= swUpdateInfo._BinarySize)
//        {
//            timer->stop();
//            // TODO: Notify that the download is completed.
//            // This signal would be emitted from the function which receives the binary file
//            // and detects that the download is complete
//            emit binaryDownloadCompleted();
//        }
//        else
//            downloadedSize += 0.1;
//    });
//    timer->start();
    //<--
}

void DeviceConnect::installUpdatedBinaries()
{
    // TODO: Start copying the installation files
    emit binaryInstallationStarted();

    //--> TODO : Temp Code.
//    float installedSize = 0;
//    QTimer *timer = new QTimer();
//    timer->setInterval(100);
//    connect(timer, &QTimer::timeout, [this, timer, installedSize]() mutable {

//        emit installationProgressChanged(installedSize);
//        if (installedSize >= swUpdateInfo._BinarySize)
//        {
//            timer->stop();
//            // TODO: Notify that the download is completed.
//            // This signal would be emitted from the function which receives the binary file
//            // and detects that the download is complete
//            emit binaryInstallationCompleted();
//        }
//        else
//            installedSize += 0.1;
//    });
//    timer->start();
    //<--
}
