#include "swupdateinstallviewmodel.h"

SwUpdateInstallViewModel::SwUpdateInstallViewModel(QSharedPointer<SwUpdateDataModel> dataModel, QObject *parent)
    : QObject(parent)
    , m_dataModel(dataModel)
    , m_totalSize(0)
    , m_installedSize(0)
    , m_installedPercentage(0)
{
    connect(m_dataModel.data(), &SwUpdateDataModel::swUpdateInstallationStarted, this, &SwUpdateInstallViewModel::onSwUpdateInstallationStarted);
    connect(m_dataModel.data(), &SwUpdateDataModel::swUpdateInstallationProgressChanged, this, &SwUpdateInstallViewModel::onSwUpdateInstallationProgressChanged);
    connect(m_dataModel.data(), &SwUpdateDataModel::swUpdateInstallationCompleted, this, &SwUpdateInstallViewModel::onSwUpdateInstallationCompleted);
}

SwUpdateInstallViewModel::~SwUpdateInstallViewModel()
{

}

QString SwUpdateInstallViewModel::title()
{
    QString title = QString("INSTALATION IN PROGRESS - %1%").arg((int)m_installedPercentage);
    return title;
}

QString SwUpdateInstallViewModel::remainingStatus()
{
    int elapsedTimeInSec = m_elapsedTimer.elapsed() / 1000;
    int mbPerSec = elapsedTimeInSec == 0 ? 0 : m_installedSize / elapsedTimeInSec;
    int remainingTime = (m_totalSize - m_installedSize) * mbPerSec;

    QString status = QString("Approx. %1 minutes remaining").arg(remainingTime == 0 ? 5 : remainingTime);
    return status;
}

uint8_t SwUpdateInstallViewModel::installedPercent()
{
    return m_installedPercentage;
}

void SwUpdateInstallViewModel::onSwUpdateInstallationStarted()
{
    emit swUpdateInstallationStarted();

    m_totalSize = m_dataModel->swUpdateInfo()._BinarySize;
    m_elapsedTimer.start();

    emit titleChanged();
    emit remainingStatusChanged();
}

void SwUpdateInstallViewModel::onSwUpdateInstallationProgressChanged(const float installedSize)
{
    if (installedSize == 0)
        return;

    m_installedSize = installedSize;
    m_installedPercentage = m_installedSize / (m_totalSize / 100);

    emit titleChanged();
    emit remainingStatusChanged();
    emit installedPercentChanged();
}

void SwUpdateInstallViewModel::onSwUpdateInstallationCompleted()
{
    m_installedSize = m_totalSize;
    m_installedPercentage = 100;

    emit titleChanged();
    emit remainingStatusChanged();
    emit installedPercentChanged();
    emit swUpdateInstallationCompleted();
}
