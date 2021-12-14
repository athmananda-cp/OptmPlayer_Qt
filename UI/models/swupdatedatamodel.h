#ifndef SWUPDATEDATAMODEL_H
#define SWUPDATEDATAMODEL_H

#include <QObject>
#include "commontypes.h"

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

signals:
    void checkForSwUpdateStarted();
    void swUpdateAvaialble(const SwUpdateInfo_t &swUpdateInfo);
    void checkForSwUpdateCompleted(bool status, QString message);

    void binaryDownloadStarted();
    void downloadProgressChanged(qint64 bytesReceived, qint64 bytesTotal);
    void binaryDownloadCompleted(bool status, QString message);

    void swUpdateInstallationStarted();
    void swUpdateInstallationProgressChanged(const float installedSize);
    void swUpdateInstallationCompleted(bool status, QString message);

private:
    SwUpdateInfo_t m_swUpdateInfo;
};

#endif // SWUPDATEDATAMODEL_H
