#ifndef DEVICECONNECT_H
#define DEVICECONNECT_H

#include <QObject>
#include "commontypes.h"

class DeviceConnect : public QObject
{
    Q_OBJECT
public:
    explicit DeviceConnect(QObject *parent = nullptr);
    void requestSwVersionInfo();
    void downloadUpdateBinaries();
    void installUpdatedBinaries();

signals:
    void swVersionInfoAvailable(const SwUpdateInfo_t &swUpdateInfo);
    void binaryDownloadStarted();
    void downloadProgressChanged(const float downloadedSize);
    void binaryDownloadCompleted();

    void binaryInstallationStarted();
    void installationProgressChanged(const float installationSize);
    void binaryInstallationCompleted();
};

#endif // DEVICECONNECT_H
