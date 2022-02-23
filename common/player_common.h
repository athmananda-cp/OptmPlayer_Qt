#ifndef PLAYER_COMMON_H
#define PLAYER_COMMON_H

#include <cstdint>
#include <QString>
#include <QStringList>
#include <QMetaType>

/*-------------------------------------------------------------------------------
 * Structures for data received from homecaster REST APIs
  -----------------------------------------------------------------------------*/
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
    int                 VersionNumber;
    QStringList         ListOfChanges;
    ObjectInfo_t        UpgradeImageObjectInfo;
} UpgradeInfo_t;

typedef struct hcasterInfo
{
    QString             IpAddress;
    QString             PlaybackUrl;
    QList<ObjectInfo_t> ObjectInfoList;
    UpgradeInfo_t       UpgradeInfo;
} HCasterInfo_t;


/*-------------------------------------------------------------------------------
 * Structures for UI
  -----------------------------------------------------------------------------*/
typedef struct versionInfo
{
    Q_GADGET
    Q_PROPERTY(std::uint16_t MajorNumber MEMBER _MajorNumber)
    Q_PROPERTY(std::uint16_t MinorNumber MEMBER _MinorNumber)
    Q_PROPERTY(std::uint32_t MaintenanceNumber MEMBER _MaintenanceNumber)

public:
    std::uint16_t     _MajorNumber;
    std::uint16_t     _MinorNumber;
    std::uint32_t     _MaintenanceNumber;

    bool equals(const struct versionInfo vInfo);
    bool operator !=(const struct versionInfo);
} VersionInfo_t;
Q_DECLARE_METATYPE(VersionInfo_t);

typedef struct swUpdateInfo
{
    Q_GADGET
    Q_PROPERTY(VersionInfo_t VersionInfo MEMBER _VersionInfo)
    Q_PROPERTY(QString Enhancements MEMBER _Enhancements)
    Q_PROPERTY(float BinarySize MEMBER _BinarySize)

public:
    VersionInfo_t     _VersionInfo;
    QString           _Enhancements;
    float             _BinarySize;
} SwUpdateInfo_t;


typedef struct ErrorDetails
{
    Q_GADGET
    Q_PROPERTY(QString ErrorInfo MEMBER _ErrorInfo)

public:
    QString           _ErrorInfo;
} errorInfo_t;

Q_DECLARE_METATYPE(SwUpdateInfo_t);

#endif // PLAYER_COMMON_H
