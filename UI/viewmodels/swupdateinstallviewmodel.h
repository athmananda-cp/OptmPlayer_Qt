#ifndef SWUPDATEINSTALLVIEWMODEL_H
#define SWUPDATEINSTALLVIEWMODEL_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QElapsedTimer>

#include "UI/models/swupdatedatamodel.h"

class SwUpdateInstallViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString remainingStatus READ remainingStatus NOTIFY remainingStatusChanged)
    Q_PROPERTY(float installedPercent READ installedPercent NOTIFY installedPercentChanged)

public:
    explicit SwUpdateInstallViewModel(QSharedPointer<SwUpdateDataModel> dataModel, QObject *parent = nullptr);
    virtual ~SwUpdateInstallViewModel();

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

private:
    QSharedPointer<SwUpdateDataModel> m_dataModel;
    float               m_totalSize;
    float               m_installedSize;
    float               m_installedPercentage;
    QElapsedTimer       m_elapsedTimer;
};

#endif // SWUPDATEINSTALLVIEWMODEL_H
