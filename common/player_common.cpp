#include "common/player_common.h"

bool versionInfo::equals(const versionInfo vInfo)
{
    return _MajorNumber == vInfo._MajorNumber
            && _MinorNumber == vInfo._MinorNumber
            && _MaintenanceNumber == vInfo._MaintenanceNumber;
}

bool versionInfo::operator !=(const versionInfo vInfo)
{
    return !(_MajorNumber == vInfo._MajorNumber
            && _MinorNumber == vInfo._MinorNumber
            && _MaintenanceNumber == vInfo._MaintenanceNumber);
}
