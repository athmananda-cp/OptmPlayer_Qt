#ifndef PLAYER_UI_VIEW_FWINFO_H
#define PLAYER_UI_VIEW_FWINFO_H

#include <QObject>
#include <QSharedPointer>

#include "UI/player_ui_data.h"

class PlayerUiViewFwInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString enhancements READ enhancements NOTIFY enhancementsChanged)
    Q_PROPERTY(QString errorInfo READ errorInfo NOTIFY errorOccured)
public:
    explicit PlayerUiViewFwInfo(QSharedPointer<PlayerUiData> dataModel, QObject *parent = nullptr);
    ~PlayerUiViewFwInfo();

    void setSwUpdateInfo(const SwUpdateInfo_t swUpdateInfo);
    void upDateErrorInfo(const errorInfo_t errorInfo);
    QString title();
    QString description();
    QString enhancements();
	QString errorInfo();
	
    void setTitle(QString title);
    void setDescription(QString description);
    void setEnhancements(QString enhancements);
    void setErrorInfo(QString errorInfo);
signals:
    void swUpdateAvailable();
	void errorOccured();
    void titleChanged();
    void descriptionChanged();
    void enhancementsChanged();

private:
    QString m_title;
    QString m_description;
    QString m_enhancements;
	QString m_errorInfo;
    QSharedPointer<PlayerUiData> m_dataModel;
};

#endif // PLAYER_UI_VIEW_FWINFO_H
