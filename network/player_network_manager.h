#ifndef PLAYER_NETWORK_MANAGER_H
#define PLAYER_NETWORK_MANAGER_H

#include <QObject>

class PlayerNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit PlayerNetworkManager(QObject *parent = nullptr);
    virtual ~PlayerNetworkManager();

    void getHCasterIpAddress();
    void getListObjects();
    void getServiceList();

signals:
    void listOfObjectsAvailable();
    void sHomeCasterStatus(qint64 iStatus);
};

#endif // PLAYER_NETWORK_MANAGER_H
