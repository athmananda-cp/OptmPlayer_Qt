#include "player_ui_view_fwdownload.h"

#include <QDebug>

PlayerUiViewFwDownload::PlayerUiViewFwDownload(QSharedPointer<PlayerUiData> dataModel, QObject *parent)
    : QObject(parent)
    , m_dataModel(dataModel)
    , m_totalSize(0)
    , m_downloadedSize(0)
    , m_downloadedPercentage(0)
{
    connect(m_dataModel.data(), &PlayerUiData::binaryDownloadStarted, this, &PlayerUiViewFwDownload::onBinaryDownloadStarted);
    connect(m_dataModel.data(), &PlayerUiData::downloadProgressChanged, this, &PlayerUiViewFwDownload::onDownloadProgressChanged);
    connect(m_dataModel.data(), &PlayerUiData::binaryDownloadCompleted, this, &PlayerUiViewFwDownload::onBinaryDownloadCompleted);
}

PlayerUiViewFwDownload::~PlayerUiViewFwDownload()
{
}

QString PlayerUiViewFwDownload::title()
{
    QString title = QString("DOWNLOAD IN PROGRESS - %1%").arg((int)m_downloadedPercentage);
    return title;
}

QString PlayerUiViewFwDownload::completionStatus()
{
    int elapsedTimeInSec = m_elapsedTimer.elapsed() / 1000;
    QString status = QString("Completed %1 Mb (%2 MB/s)")
            .arg(m_downloadedSize)
            .arg(elapsedTimeInSec == 0 ? 0 : m_downloadedSize / elapsedTimeInSec);
    return status;
}

QString PlayerUiViewFwDownload::remainingStatus()
{
    int elapsedTimeInSec = m_elapsedTimer.elapsed() / 1000;
    int mbPerSec = elapsedTimeInSec == 0 ? 0 : m_downloadedSize / elapsedTimeInSec;
    int remainingTime = (m_totalSize - m_downloadedSize) * mbPerSec;
    QString status = QString("Remaining %1 MB (%2 minutes)")
            .arg(m_totalSize - m_downloadedSize)
            .arg(remainingTime == 0 ? 5 : remainingTime);
    return status;
}

uint8_t PlayerUiViewFwDownload::downloadedPercent()
{
    return m_downloadedPercentage;
}

void PlayerUiViewFwDownload::onBinaryDownloadStarted()
{
    emit swUpdatedDownloadStarted();

    m_totalSize = m_dataModel->swUpdateInfo()._BinarySize;
    m_elapsedTimer.start();

    emit titleChanged();
    emit completionStatusChanged();
    emit downloadedPercentChanged();
    emit remainingStatusChanged();
}

void PlayerUiViewFwDownload::onDownloadProgressChanged(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesReceived == 0 || bytesTotal == 0)
        return;

    m_downloadedSize = bytesReceived;
    m_downloadedPercentage = m_downloadedSize / (bytesTotal / 100);

    emit titleChanged();
    emit completionStatusChanged();
    emit downloadedPercentChanged();
    emit remainingStatusChanged();
}

void PlayerUiViewFwDownload::onBinaryDownloadCompleted()
{
    m_downloadedSize = m_totalSize;
    m_downloadedPercentage = 100;

    emit titleChanged();
    emit completionStatusChanged();
    emit downloadedPercentChanged();
    emit remainingStatusChanged();

    emit swUpdateDownloadCompleted();
}
