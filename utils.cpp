#include "utils.h"

#include <QSettings>
#include <QFile>
#include <QDebug>

const VersionInfo_t readCurrentVersionInfo()
{
    VersionInfo_t versionInfo;
    QFile configFile("..//OptmPlayer_Qt//config//versioninfo.ini");
    if (configFile.exists())
    {
        QSettings settings("..//OptmPlayer_Qt//config//versioninfo.ini", QSettings::IniFormat);
        versionInfo._MajorNumber = settings.value("CurrentVersion/major").toUInt();
        versionInfo._MinorNumber = settings.value("CurrentVersion/minor").toUInt();
        versionInfo._MaintenanceNumber = settings.value("CurrentVersion/maintenance").toUInt();

//        qDebug() << QString("Current version info fetched is %1.%2.%3")
//                    .arg(versionInfo._MajorNumber)
//                    .arg(versionInfo._MinorNumber)
//                    .arg(versionInfo._MaintenanceNumber);
    }
    else
        qDebug() << "Couldn't open versioninfo.ini file";

    return versionInfo;
}
