#include "swupdatedownloadviewmodel.h"

#include <QDebug>

SwUpdateDownloadViewModel::SwUpdateDownloadViewModel(QSharedPointer<SwUpdateDataModel> dataModel, QObject *parent)
    : QObject(parent)
    , m_dataModel(dataModel)
    , m_totalSize(0)
    , m_downloadedSize(0)
    , m_downloadedPercentage(0)
{
    connect(m_dataModel.data(), &SwUpdateDataModel::binaryDownloadStarted, this, &SwUpdateDownloadViewModel::onBinaryDownloadStarted);
    connect(m_dataModel.data(), &SwUpdateDataModel::downloadProgressChanged, this, &SwUpdateDownloadViewModel::onDownloadProgressChanged);
    connect(m_dataModel.data(), &SwUpdateDataModel::binaryDownloadCompleted, this, &SwUpdateDownloadViewModel::onBinaryDownloadCompleted);
}

SwUpdateDownloadViewModel::~SwUpdateDownloadViewModel()
{
}

QString SwUpdateDownloadViewModel::title()
{
    QString title = QString("DOWNLOAD IN PROGRESS - %1%").arg((int)m_downloadedPercentage);
    return title;
}

QString SwUpdateDownloadViewModel::completionStatus()
{
    int elapsedTimeInSec = m_elapsedTimer.elapsed() / 1000;
    QString status = QString("Completed %1 Mb (%2 MB/s)")
            .arg(m_downloadedSize)
            .arg(elapsedTimeInSec == 0 ? 0 : m_downloadedSize / elapsedTimeInSec);
    return status;
}

QString SwUpdateDownloadViewModel::remainingStatus()
{
    int elapsedTimeInSec = m_elapsedTimer.elapsed() / 1000;
    int mbPerSec = elapsedTimeInSec == 0 ? 0 : m_downloadedSize / elapsedTimeInSec;
    int remainingTime = (m_totalSize - m_downloadedSize) * mbPerSec;
    QString status = QString("Remaining %1 MB (%2 minutes)")
            .arg(m_totalSize - m_downloadedSize)
            .arg(remainingTime == 0 ? 5 : remainingTime);
    return status;
}

uint8_t SwUpdateDownloadViewModel::downloadedPercent()
{
    return m_downloadedPercentage;
}

void SwUpdateDownloadViewModel::onBinaryDownloadStarted()
{
    m_totalSize = m_dataModel->swUpdateInfo()._BinarySize;
    m_elapsedTimer.start();

    emit titleChanged();
    emit completionStatusChanged();
    emit downloadedPercentChanged();
    emit remainingStatusChanged();
}

void SwUpdateDownloadViewModel::onDownloadProgressChanged(const float downloadedSize)
{
    if (downloadedSize == 0)
        return;

    m_downloadedSize = downloadedSize;
    m_downloadedPercentage = m_downloadedSize / (m_totalSize / 100);

    emit titleChanged();
    emit completionStatusChanged();
    emit downloadedPercentChanged();
    emit remainingStatusChanged();
}

void SwUpdateDownloadViewModel::onBinaryDownloadCompleted()
{
    m_downloadedSize = m_totalSize;
    m_downloadedPercentage = 100;

    emit titleChanged();
    emit completionStatusChanged();
    emit downloadedPercentChanged();
    emit remainingStatusChanged();
}
