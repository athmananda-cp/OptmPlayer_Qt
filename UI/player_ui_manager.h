#ifndef PLAYER_UI_MANAGER_H
#define PLAYER_UI_MANAGER_H

#include "UI/player_ui_data.h"
#include "UI/viewmodels/player_ui_view_fwinfo.h"
#include "UI/viewmodels/player_ui_view_fwdownload.h"
#include "UI/viewmodels/player_ui_view_fwinstall.h"

#include <QSharedPointer>
#include <QObject>

class PlayerUIManager : public QObject
{
    Q_OBJECT
public:
    explicit PlayerUIManager(QObject *parent = nullptr);
    void registerMetaTypes();
    void checkForUpdates();
    void updateError(QString ErrorMessage);
    Q_INVOKABLE void downloadUpdate();
    Q_INVOKABLE void installUpdate();

private:
    QSharedPointer<PlayerUiData> m_updateDataModel {nullptr};
    PlayerUiViewFwInfo *m_updateInfoViewModel {nullptr};
    PlayerUiViewFwDownload *m_downloadViewModel {nullptr};
    PlayerUiViewFwInstall *m_installViewModel {nullptr};
};

#endif // PLAYER_UI_MANAGER_H
