#ifndef SWUPDATEMANAGER_H
#define SWUPDATEMANAGER_H

#include "UI/models/swupdatedatamodel.h"
#include "UI/viewmodels/swupdateinfoviewmodel.h"
#include "UI/viewmodels/swupdatedownloadviewmodel.h"
#include "UI/viewmodels/swupdateinstallviewmodel.h"

#include <QSharedPointer>
#include <QObject>

class SwUpdateManager : public QObject
{
    Q_OBJECT
public:
    explicit SwUpdateManager(QObject *parent = nullptr);
    void registerMetaTypes();
    void checkForUpdates();
    Q_INVOKABLE void downloadUpdate();
    Q_INVOKABLE void installUpdate();

signals:
    void swUpdateCheckStarted();
    void swUpdateCheckFailed();
    void swUpdateAvailable();
    void swUpdateCheckCompleted();

    void swUpdatedDownloadStarted();
    void swUpdateDownloadFailed();
    void swUpdateDownloadCompleted();

    void swUpdateInstallationStarted();
    void swUpdateInstallationFailed();
    void swUpdateInstallationCompleted();

private slots:
    void onSwUpdateAvaialble(const SwUpdateInfo_t &swUpdateInfo);

private:
    QSharedPointer<SwUpdateDataModel> m_updateDataModel {nullptr};
    SwUpdateInfoViewModel *m_updateInfoViewModel {nullptr};
    SwUpdateDownloadViewModel *m_downloadViewModel {nullptr};
    SwUpdateInstallViewModel *m_installViewModel {nullptr};
};

#endif // SWUPDATEMANAGER_H
