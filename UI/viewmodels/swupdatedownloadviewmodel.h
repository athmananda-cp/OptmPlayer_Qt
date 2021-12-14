#ifndef SWUPDATEDOWNLOADVIEWMODEL_H
#define SWUPDATEDOWNLOADVIEWMODEL_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QElapsedTimer>

#include "UI/models/swupdatedatamodel.h"

class SwUpdateDownloadViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString completionStatus READ completionStatus NOTIFY completionStatusChanged)
    Q_PROPERTY(QString remainingStatus READ remainingStatus NOTIFY remainingStatusChanged)
    Q_PROPERTY(float downloadedPercent READ downloadedPercent NOTIFY downloadedPercentChanged)

public:
    explicit SwUpdateDownloadViewModel(QSharedPointer<SwUpdateDataModel> dataModel, QObject *parent = nullptr);
    virtual ~SwUpdateDownloadViewModel();

public slots:
    QString title();
    QString completionStatus();
    QString remainingStatus();
    uint8_t downloadedPercent();

    void onBinaryDownloadStarted();
    void onDownloadProgressChanged(qint64 bytesReceived, qint64 bytesTotal);
    void onBinaryDownloadCompleted();

signals:
    void swUpdatedDownloadStarted();
    void swUpdateDownloadFailed();
    void swUpdateDownloadCompleted();

    void titleChanged();
    void completionStatusChanged();
    void remainingStatusChanged();
    void downloadedPercentChanged();

private:
    QSharedPointer<SwUpdateDataModel> m_dataModel;
    float               m_totalSize;
    float               m_downloadedSize;
    float               m_downloadedPercentage;
    QElapsedTimer       m_elapsedTimer;
};

#endif // SWUPDATEDOWNLOADVIEWMODEL_H
