#include "utils.h"
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QFileInfo>

const VersionInfo_t readCurrentVersionInfo()
{
    VersionInfo_t versionInfo;
    QFileInfo appExe(QCoreApplication::applicationFilePath());
    QFile configFile(appExe.canonicalPath() + "/versioninfo.ini");
    if (configFile.exists())
    {
        QSettings settings(appExe.canonicalPath() + "/versioninfo.ini", QSettings::IniFormat);
        QString strVersionInfo = settings.value("CurrentVersion/number").toString();
        if (strVersionInfo.isEmpty())
        {
            qDebug() << "No current version info found in the versioninfo.ini file";
            return versionInfo;
        }

        QStringList versionNumbers = strVersionInfo.split(".");
        if (versionNumbers.length() != 3)
        {
            qDebug() << "Invalid version info found in versioninfo.ini as: " << strVersionInfo;
            return versionInfo;
        }

        versionInfo._MajorNumber       = versionNumbers[0].toInt();
        versionInfo._MinorNumber       = versionNumbers[1].toInt();
        versionInfo._MaintenanceNumber = versionNumbers[2].toInt();

        qDebug() << QString("Current version info fetched is %1.%2.%3")
                    .arg(versionInfo._MajorNumber)
                    .arg(versionInfo._MinorNumber)
                    .arg(versionInfo._MaintenanceNumber);
    }
    else
        qDebug() << "Couldn't open versioninfo.ini file. Make sure that versioninfo.ini file is in the same folder as the executable.";

    return versionInfo;
}

void updateVersionInfo(const SwUpdateInfo_t swUpdateInfo)
{
    QFileInfo appExe(QCoreApplication::applicationFilePath());
    QFile configFile(appExe.canonicalPath() + "/versioninfo.ini");
    if (configFile.exists())
    {
        QSettings settings(appExe.canonicalPath() + "/versioninfo.ini", QSettings::IniFormat);
        settings.beginGroup("CurrentVersion");
        settings.setValue("number", QString("%1.%2.%3").arg(swUpdateInfo._VersionInfo._MajorNumber)
                          .arg(swUpdateInfo._VersionInfo._MinorNumber)
                          .arg(swUpdateInfo._VersionInfo._MaintenanceNumber));
        settings.setValue("changes", swUpdateInfo._Enhancements);
        settings.endGroup();
        settings.sync();
    }
    else
        qDebug() << "Updating versioninfo.ini failed";
}
