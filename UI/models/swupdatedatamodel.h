#ifndef SWUPDATEDATAMODEL_H
#define SWUPDATEDATAMODEL_H

#include <QObject>
#include "deviceconnect.h"

class SwUpdateDataModel : public QObject
{
    Q_OBJECT
public:
    explicit SwUpdateDataModel(QObject *parent = nullptr);
    virtual ~SwUpdateDataModel();
    void checkForSwUpdate();
    void performDownload();
    void performInstallation();

    const SwUpdateInfo_t &swUpdateInfo() const;

private slots:
    void onSwVersionInfoAvailable(const SwUpdateInfo_t &swUpdateInfo);

signals:
    void swUpdateAvaialble(const SwUpdateInfo_t &swUpdateInfo);

    void binaryDownloadStarted();
    void downloadProgressChanged(const float downloadedSize);
    void binaryDownloadCompleted();

    void swUpdateInstallationStarted();
    void swUpdateInstallationProgressChanged(const float installedSize);
    void swUpdateInstallationCompleted();

private:
    DeviceConnect  m_deviceConnect;
    SwUpdateInfo_t m_swUpdateInfo;
};

#endif // SWUPDATEDATAMODEL_H
