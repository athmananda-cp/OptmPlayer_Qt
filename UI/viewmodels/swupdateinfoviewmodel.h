#ifndef SWUPDATEINFOVIEWMODEL_H
#define SWUPDATEINFOVIEWMODEL_H

#include <QObject>
#include <QSharedPointer>

#include "UI/models/swupdatedatamodel.h"

class SwUpdateInfoViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString enhancements READ enhancements NOTIFY enhancementsChanged)

public:
    explicit SwUpdateInfoViewModel(QSharedPointer<SwUpdateDataModel> dataModel, QObject *parent = nullptr);
    ~SwUpdateInfoViewModel();

    void setSwUpdateInfo(const SwUpdateInfo_t swUpdateInfo);

    QString title();
    QString description();
    QString enhancements();

    void setTitle(QString title);
    void setDescription(QString description);
    void setEnhancements(QString enhancements);

signals:
    void swUpdateAvailable();

    void titleChanged();
    void descriptionChanged();
    void enhancementsChanged();

private:
    QString m_title;
    QString m_description;
    QString m_enhancements;

    QSharedPointer<SwUpdateDataModel> m_dataModel;
};

#endif // SWUPDATEINFOVIEWMODEL_H
