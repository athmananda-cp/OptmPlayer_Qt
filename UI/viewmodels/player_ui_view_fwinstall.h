#ifndef PLAYER_UI_VIEW_FWINSTALL_H
#define PLAYER_UI_VIEW_FWINSTALL_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QElapsedTimer>

#include "UI/player_ui_data.h"

class PlayerUiViewFwInstall : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString remainingStatus READ remainingStatus NOTIFY remainingStatusChanged)
    Q_PROPERTY(float installedPercent READ installedPercent NOTIFY installedPercentChanged)

public:
    explicit PlayerUiViewFwInstall(QSharedPointer<PlayerUiData> dataModel, QObject *parent = nullptr);
    virtual ~PlayerUiViewFwInstall();

public slots:
    QString title();
    QString remainingStatus();
    uint8_t installedPercent();

    void onSwUpdateInstallationStarted();
    void onSwUpdateInstallationProgressChanged(const float installedSize);
    void onSwUpdateInstallationCompleted();

signals:
    void titleChanged();
    void remainingStatusChanged();
    void installedPercentChanged();

    void swUpdateInstallationStarted();
    void swUpdateInstallationFailed();
    void swUpdateInstallationCompleted();

private:
    QSharedPointer<PlayerUiData> m_dataModel;
    float               m_totalSize;
    float               m_installedSize;
    float               m_installedPercentage;
    QElapsedTimer       m_elapsedTimer;
};

#endif // PLAYER_UI_VIEW_FWINSTALL_H
