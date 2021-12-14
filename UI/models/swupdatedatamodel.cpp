#include "swupdatedatamodel.h"
#include "network/networkmanager.h"
#include "network/requests.h"
#include "player.h"
#include "utils.h"
#include <QSettings>
#include <QDir>
#include <QDebug>

SwUpdateDataModel::SwUpdateDataModel(QObject *parent) : QObject(parent)
{
}

SwUpdateDataModel::~SwUpdateDataModel()
{

}

void SwUpdateDataModel::checkForSwUpdate()
{
    emit checkForSwUpdateStarted();
    IRequest *request = new GetUpgradeJsonRequest(this);
    connect(request, &IRequest::requestCompleted, this, [this, request](IRequest::Status status) {
        switch (status)
        {
        case IRequest::Status::TimedOut:
        {
            qDebug() << "Request to get upgrade json timed out";
            emit checkForSwUpdateCompleted(false, "Request to get upgrade json timed out");
            break;
        }
        case IRequest::Status::Success:
        {
            SwUpdateInfo_t swUpdateInfo;
            swUpdateInfo._VersionInfo._MajorNumber = qApp->hCasterInfo()->UpgradeInfo.MajorVersion;
            swUpdateInfo._VersionInfo._MinorNumber = qApp->hCasterInfo()->UpgradeInfo.MinorVersion;
            swUpdateInfo._VersionInfo._MaintenanceNumber = 0;
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
        case IRequest::Status::Failed:
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

void SwUpdateDataModel::performDownload()
{
    emit binaryDownloadStarted();
    IRequest *request = new GetBinaryImageRequest(this);
    connect(request, &IRequest::downloadProgress, this, &SwUpdateDataModel::downloadProgressChanged);
    connect(request, &IRequest::requestCompleted, this, [this, request](IRequest::Status status) {
        switch (status)
        {
        case IRequest::Status::TimedOut:
        {
            qDebug() << "Request to get binary timed out";
            emit binaryDownloadCompleted(false, "Request to get binary timed out");
            break;
        }
        case IRequest::Status::Success:
        {
            qDebug() << "Get binary image request completed";
            emit binaryDownloadCompleted(true, "Request to get binary completed");
            performInstallation();
            break;
        }
        case IRequest::Status::Failed:
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

void SwUpdateDataModel::performInstallation()
{
    emit swUpdateInstallationStarted();
    // Take backup of existing binary
    QString appExeFile = QCoreApplication::applicationFilePath();
    QString backupFile = appExeFile + ".bkp";
    QFile::rename(appExeFile, backupFile);
    emit swUpdateInstallationProgressChanged(10);

    // Copy new binary
    QFile downloadedFile("/tmp/" + QCoreApplication::applicationName());
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

const SwUpdateInfo_t &SwUpdateDataModel::swUpdateInfo() const
{
    return m_swUpdateInfo;
}
