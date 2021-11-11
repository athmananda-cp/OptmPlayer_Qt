#ifndef COMMONTYPES_H
#define COMMONTYPES_H

#include <cstdint>
#include <QString>
#include <QMetaType>

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
}VersionInfo_t;
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
}SwUpdateInfo_t;
Q_DECLARE_METATYPE(SwUpdateInfo_t);

#endif // COMMONTYPES_H
