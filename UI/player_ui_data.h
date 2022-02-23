#ifndef PLAYER_UI_DATA_H
#define PLAYER_UI_DATA_H

#include <QObject>
#include "common/player_common.h"

class PlayerUiData : public QObject
{
    Q_OBJECT
public:
    explicit PlayerUiData(QObject *parent = nullptr);
    virtual ~PlayerUiData();
    void checkForSwUpdate();
    void performDownload();
    void performInstallation();
    void updateError(QString ErrorMessage);
    const SwUpdateInfo_t &swUpdateInfo() const;

signals:
    void checkForSwUpdateStarted();
    void swUpdateAvaialble(const SwUpdateInfo_t &swUpdateInfo);
    void checkForSwUpdateCompleted(bool status, QString message);
    void errorOccured(const errorInfo_t &errorInfo);	
	
    void binaryDownloadStarted();
    void downloadProgressChanged(qint64 bytesReceived, qint64 bytesTotal);
    void binaryDownloadCompleted(bool status, QString message);

    void swUpdateInstallationStarted();
    void swUpdateInstallationProgressChanged(const float installedSize);
    void swUpdateInstallationCompleted(bool status, QString message);

private:
    SwUpdateInfo_t m_swUpdateInfo;
};

#endif // PLAYER_UI_DATA_H
