#include "UI/player_ui_data.h"
#include "network/player_network_manager.h"
#include "network/player_network_requests.h"
#include "player.h"
#include "common/player_utils.h"
#include <QSettings>
#include <QDir>
#include <QDebug>

PlayerUiData::PlayerUiData(QObject *parent) : QObject(parent)
{
}

PlayerUiData::~PlayerUiData()
{

}

void PlayerUiData::checkForSwUpdate()
{
    emit checkForSwUpdateStarted();
    IPlayerNetworkRequest *request = new PlayerHcGetUpgradeJson(this);
    connect(request, &IPlayerNetworkRequest::requestCompleted, this, [this, request](IPlayerNetworkRequest::Status status) {
        switch (status)
        {
        case IPlayerNetworkRequest::Status::TimedOut:
        {
            qDebug() << "Request to get upgrade json timed out";
            emit checkForSwUpdateCompleted(false, "Request to get upgrade json timed out");
            break;
        }
        case IPlayerNetworkRequest::Status::Success:
        {
            SwUpdateInfo_t swUpdateInfo;
            swUpdateInfo._VersionInfo._MajorNumber = qApp->hCasterInfo()->UpgradeInfo.MajorVersion;
            swUpdateInfo._VersionInfo._MinorNumber = qApp->hCasterInfo()->UpgradeInfo.MinorVersion;
            swUpdateInfo._VersionInfo._MaintenanceNumber = qApp->hCasterInfo()->UpgradeInfo.VersionNumber;
            swUpdateInfo._Enhancements = qApp->hCasterInfo()->UpgradeInfo.ListOfChanges.join("");

            m_swUpdateInfo = swUpdateInfo;

            // Verify if SW update is needed
            VersionInfo_t versionInfo = readCurrentVersionInfo();
            if (! versionInfo.equals(swUpdateInfo._VersionInfo))
            {
                emit swUpdateAvaialble(swUpdateInfo);
                emit checkForSwUpdateCompleted(true, "update available");
            }
            else
            {
                qDebug() << "Software upto date. No update required";
                emit checkForSwUpdateCompleted(true, "SW version upto date");
            }
            break;
        }
        case IPlayerNetworkRequest::Status::Failed:
        {
            qDebug() << "Request to get upgrade json failed";
            emit checkForSwUpdateCompleted(false, "Request to get upgrade json failed");
            break;
        }
        }

        delete request;
    });
    request->execute();
}

void PlayerUiData::performDownload()
{
    emit binaryDownloadStarted();
    IPlayerNetworkRequest *request = new PlayerHcGetUpgradeImage(this);
    connect(request, &IPlayerNetworkRequest::downloadProgress, this, &PlayerUiData::downloadProgressChanged);
    connect(request, &IPlayerNetworkRequest::requestCompleted, this, [this, request](IPlayerNetworkRequest::Status status) {
        switch (status)
        {
        case IPlayerNetworkRequest::Status::TimedOut:
        {
            qDebug() << "Request to get binary timed out";
            emit binaryDownloadCompleted(false, "Request to get binary timed out");
            break;
        }
        case IPlayerNetworkRequest::Status::Success:
        {
            qDebug() << "Get binary image request completed";
            emit binaryDownloadCompleted(true, "Request to get binary completed");
            performInstallation();
            break;
        }
        case IPlayerNetworkRequest::Status::Failed:
        {
            qDebug() << "Request to get binary failed";
            emit binaryDownloadCompleted(false, "Request to get binary failed");
            break;
        }
        }

        delete request;
    });
    request->execute();
}

void PlayerUiData::performInstallation()
{
    emit swUpdateInstallationStarted();
    // Take backup of existing binary
    QString appExeFile = QCoreApplication::applicationFilePath();
    QString backupFile = appExeFile + ".bkp";
    QFile::remove(backupFile);
    QFile::rename(appExeFile, backupFile);
    emit swUpdateInstallationProgressChanged(10);

    // Copy new binary
    QFile downloadedFile("/home/savi/project/" + QCoreApplication::applicationName());
    if (! downloadedFile.exists())
    {
        qDebug() << "File " << downloadedFile.fileName() << " not found. Installation aborted.";
        emit swUpdateInstallationCompleted(false, "Source file not found");
        return;
    }

    if(downloadedFile.open(QFile::ReadOnly))
    {
        QFile appFile(appExeFile);
        if(appFile.open(QFile::WriteOnly))
        {
            const qint64 filesize = downloadedFile.size();
            int totalBytesRead = 0;
            char data[1024 * 1024];
            while (totalBytesRead < filesize)
            {
                int bytesRead = downloadedFile.read(&data[0], 1024 * 1024);
                if (bytesRead > 0)
                    appFile.write(&data[0], bytesRead);
                totalBytesRead += bytesRead;
            }
            appFile.setPermissions(QFile::ReadOwner
                                   | QFile::WriteOwner
                                   | QFile::ExeOwner
                                   | QFile::ReadUser
                                   | QFile::WriteUser
                                   | QFile::ExeUser
                                   | QFile::ReadGroup
                                   | QFile::WriteGroup
                                   | QFile::ExeGroup
                                   | QFile::ReadOther
                                   | QFile::ExeOther);
            appFile.close();
        }
        else
        {
            qDebug() << "Couldn't open " << appFile.fileName() << " for writing";
            emit swUpdateInstallationCompleted(false, "Could not open file for writing");
            return;
        }

        appFile.close();
        emit swUpdateInstallationProgressChanged(100);
    }
    else
    {
        qDebug() << "Couldn't open " << downloadedFile.fileName() << " for reading";
        emit swUpdateInstallationCompleted(false, "Could not open file for reading");
        return;
    }

    // Update the version info
    updateVersionInfo(m_swUpdateInfo);

    emit swUpdateInstallationCompleted(true, "Software installation completed.");
}

void PlayerUiData::updateError(QString ErrorMessage)
{
    errorInfo_t errorInfo;
    errorInfo._ErrorInfo = ErrorMessage;
    emit errorOccured(errorInfo);
}

const SwUpdateInfo_t &PlayerUiData::swUpdateInfo() const
{
    return m_swUpdateInfo;
}
