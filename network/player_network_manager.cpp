#include "player_network_manager.h"
#include "player_network_requests.h"
#include "player.h"

PlayerNetworkManager::PlayerNetworkManager(QObject *parent)
    : QObject(parent)
{
}

PlayerNetworkManager::~PlayerNetworkManager()
{

}

void PlayerNetworkManager::getHCasterIpAddress()
{
    IPlayerNetworkRequest *request = new PlayerHcGetParams(this);
    connect(request, &IPlayerNetworkRequest::requestCompleted, this, [this, request](IPlayerNetworkRequest::Status status) {
        switch (status)
        {
            case IPlayerNetworkRequest::Status::Success:
            {
                getServiceList();
            }
            break;
            case IPlayerNetworkRequest::Status::TimedOut:
            case IPlayerNetworkRequest::Status::Failed:
            {
                emit sHomeCasterStatus(1);
               // qDebug() << "Failed to get Homecaster IP  " ;
            }
            break;
        }
        delete request;
    });
    request->execute();
}

void PlayerNetworkManager::getServiceList()
{
    IPlayerNetworkRequest *request = new PlayerHcGetServiceList(this);
    connect(request, &IPlayerNetworkRequest::requestCompleted, this, [this, request](IPlayerNetworkRequest::Status status) {
        switch (status)
        {
            case IPlayerNetworkRequest::Status::Success:
            {
                emit sHomeCasterStatus(0);
               // getListObjects();
            }
            break;
            case IPlayerNetworkRequest::Status::TimedOut:
            case IPlayerNetworkRequest::Status::Failed:
            {
                emit sHomeCasterStatus(1);
               // qDebug() << "Failed to get serviceList  " ;
            }
            break;
        }
        delete request;
    });
    request->execute();
}

void PlayerNetworkManager::getListObjects()
{
    IPlayerNetworkRequest *request = new PlayerHcGetObjectList(this);
    connect(request, &IPlayerNetworkRequest::requestCompleted, this, [this, request](IPlayerNetworkRequest::Status status) {
        switch (status)
        {
            case IPlayerNetworkRequest::Status::Success:
            {
                emit listOfObjectsAvailable();
            }
            break;
            case IPlayerNetworkRequest::Status::TimedOut:
            case IPlayerNetworkRequest::Status::Failed:
            {
                qDebug() << "Request to get List of objects timed out/failed";
            }
           break;
        }

        delete request;
    });
    request->execute();
}
