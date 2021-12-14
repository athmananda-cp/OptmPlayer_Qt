#include "swupdateinfoviewmodel.h"
#include "utils.h"

#include <QDebug>

SwUpdateInfoViewModel::SwUpdateInfoViewModel(QSharedPointer<SwUpdateDataModel> dataModel, QObject *parent)
    : QObject(parent)
    , m_dataModel(dataModel)
{
    // Handle sw update check signals
    connect(m_dataModel.data(), &SwUpdateDataModel::checkForSwUpdateStarted, this, [=]() {
        // NO OP
    });
    connect(m_dataModel.data(), &SwUpdateDataModel::swUpdateAvaialble, this, [=](const SwUpdateInfo_t &swUpdateInfo) {
        setSwUpdateInfo(swUpdateInfo);
        emit swUpdateAvailable(); // Caught in QML to show the update dialog
    });
    connect(m_dataModel.data(), &SwUpdateDataModel::checkForSwUpdateCompleted, this, [=](bool status, QString message) {
        Q_UNUSED(status);
        Q_UNUSED(message);
    });
}

SwUpdateInfoViewModel::~SwUpdateInfoViewModel()
{
}

void SwUpdateInfoViewModel::setSwUpdateInfo(const SwUpdateInfo_t swUpdateInfo)
{
    VersionInfo_t currentVersion = readCurrentVersionInfo();

    setTitle("NEW UPDATE AVAILABLE");
    setDescription(QString("New version %1.%2.%3 is available. You have %4.%5.%6")
            .arg(swUpdateInfo._VersionInfo._MajorNumber)
            .arg(swUpdateInfo._VersionInfo._MinorNumber)
            .arg(swUpdateInfo._VersionInfo._MaintenanceNumber)
            .arg(currentVersion._MajorNumber)
            .arg(currentVersion._MinorNumber)
            .arg(currentVersion._MaintenanceNumber));
    setEnhancements(swUpdateInfo._Enhancements);
}

QString SwUpdateInfoViewModel::title()
{
    return m_title;
}

QString SwUpdateInfoViewModel::description()
{
    return m_description;
}

QString SwUpdateInfoViewModel::enhancements()
{
    return m_enhancements;
}

void SwUpdateInfoViewModel::setTitle(QString title)
{
    m_title = title;
    emit titleChanged();
}

void SwUpdateInfoViewModel::setDescription(QString description)
{
    m_description = description;
    emit descriptionChanged();
}

void SwUpdateInfoViewModel::setEnhancements(QString enhancements)
{
    m_enhancements = enhancements;
    emit enhancementsChanged();
}
