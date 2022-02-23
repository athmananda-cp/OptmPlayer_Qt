#include "player_ui_view_fwinfo.h"
#include "common/player_utils.h"

#include <QDebug>

 PlayerUiViewFwInfo::PlayerUiViewFwInfo(QSharedPointer<PlayerUiData> dataModel, QObject *parent)
    : QObject(parent)
    , m_dataModel(dataModel)
{
    // Handle sw update check signals
    connect(m_dataModel.data(), &PlayerUiData::checkForSwUpdateStarted, this, [=]() {
        // NO OP
    });
    
	connect(m_dataModel.data(), &PlayerUiData::swUpdateAvaialble, this, [=](const SwUpdateInfo_t &swUpdateInfo) {
        setSwUpdateInfo(swUpdateInfo);
        emit swUpdateAvailable(); // Caught in QML to show the update dialog
    });
	
    connect(m_dataModel.data(), &PlayerUiData::checkForSwUpdateCompleted, this, [=](bool status, QString message) {
        Q_UNUSED(status);
        Q_UNUSED(message);
    });
	
    connect(m_dataModel.data(), &PlayerUiData::errorOccured, this, [=](const errorInfo_t &errorInfo) {
        upDateErrorInfo(errorInfo);
        emit errorOccured();
    });	
}

PlayerUiViewFwInfo::~PlayerUiViewFwInfo()
{
}

void PlayerUiViewFwInfo::upDateErrorInfo(const errorInfo_t errorInfo)
{
    m_errorInfo = errorInfo._ErrorInfo;
    //     Q_UNUSED(errorInfo);
}
void PlayerUiViewFwInfo::setSwUpdateInfo(const SwUpdateInfo_t swUpdateInfo)
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

QString PlayerUiViewFwInfo::title()
{
    return m_title;
}

QString PlayerUiViewFwInfo::description()
{
    return m_description;
}

QString PlayerUiViewFwInfo::enhancements()
{
    return m_enhancements;
}

QString PlayerUiViewFwInfo::errorInfo()
{
    return m_errorInfo;
}

void PlayerUiViewFwInfo::setTitle(QString title)
{
    m_title = title;
    emit titleChanged();
}

void PlayerUiViewFwInfo::setDescription(QString description)
{
    m_description = description;
    emit descriptionChanged();
}

void PlayerUiViewFwInfo::setEnhancements(QString enhancements)
{
    m_enhancements = enhancements;
    emit enhancementsChanged();
}

void PlayerUiViewFwInfo::setErrorInfo(QString errorInfo)
{
    m_errorInfo = errorInfo;
    emit errorOccured();
}
