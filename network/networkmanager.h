#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QSharedPointer>

typedef struct objectInfo
{
    QString             Url;
    QString             AppContextIdList;
    long                ContentLength;
    QString             ContentLocation;
    QString             ContentType;
    QString             DownloadStatus;
    long                Expires;
    long                NumOfBytesReceived;
    long                ReceivedTime;
    long                TransferLength;
} ObjectInfo_t;

typedef struct upgradeInfo
{
    int                 MajorVersion;
    int                 MinorVersion;
    QStringList         ListOfChanges;
    ObjectInfo_t        UpgradeImageObjectInfo;
} UpgradeInfo_t;

typedef struct hcasterInfo
{
    QString             IpAddress;
    QList<ObjectInfo_t> ObjectInfoList;
    UpgradeInfo_t       UpgradeInfo;
} HCasterInfo_t;

class IRequest;
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    virtual ~NetworkManager();
    QSharedPointer<HCasterInfo_t> hCasterInfo() const;

    void getHCasterIpAddress();
    void getListObjects();
    void getUpgradeJson();

signals:
    void listOfObjectsAvailable();
    void upgradeInfoAvailable(const UpgradeInfo_t);

private:
    QSharedPointer<HCasterInfo_t> m_hCasterInfo;
};

#endif // NETWORKMANAGER_H
