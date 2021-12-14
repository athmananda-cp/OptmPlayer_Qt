#include "networkmanager.h"
#include "requests.h"
#include "player.h"

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
{
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::getHCasterIpAddress()
{
    IRequest *request = new GetHomeCasterIpAddressRequest(this);
    connect(request, &IRequest::requestCompleted, this, [this, request](IRequest::Status status) {
        switch (status)
        {
            case IRequest::Status::Success:
            {
                getListObjects();
            }
            break;
            case IRequest::Status::TimedOut:
            case IRequest::Status::Failed:
            {
                qDebug() << "Failed to get Homecaster IP  " ;
            }
            break;
        }
        delete request;
    });
    request->execute();
}

void NetworkManager::getListObjects()
{
    IRequest *request = new GetListObjectsRequest(this);
    connect(request, &IRequest::requestCompleted, this, [this, request](IRequest::Status status) {
        switch (status)
        {
            case IRequest::Status::Success:
            {
                emit listOfObjectsAvailable();
            }
            break;
            case IRequest::Status::TimedOut:
            case IRequest::Status::Failed:
            {
                qDebug() << "Request to get List of objects timed out/failed";
            }
           break;
        }

        delete request;
    });
    request->execute();
}
