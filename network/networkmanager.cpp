#include "networkmanager.h"
#include "requests.h"
#include "player.h"

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_hCasterInfo(new HCasterInfo_t())
{
}

NetworkManager::~NetworkManager()
{

}

QSharedPointer<HCasterInfo_t> NetworkManager::hCasterInfo() const
{
    return m_hCasterInfo;
}

void NetworkManager::getHCasterIpAddress()
{
    IRequest *request = new GetHomeCasterIpAddressRequest(this);
    connect(request, &IRequest::requestCompleted, this, [this, request](IRequest::Status status) {
        switch (status)
        {
        case IRequest::Status::TimedOut:
        {
            qDebug() << "Request to get HCaster Ip address timed out";
            break;
        }
        case IRequest::Status::Success:
        {
            getListObjects();
            break;
        }
        case IRequest::Status::Failed:
        {
            qDebug() << "Request to get HCaster Ip address failed";
            break;
        }
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
        case IRequest::Status::TimedOut:
        {
            qDebug() << "Request to get List of objects timed out";
            break;
        }
        case IRequest::Status::Success:
        {
            emit listOfObjectsAvailable();
            break;
        }
        case IRequest::Status::Failed:
        {
            qDebug() << "Request to get List of objects failed";
            break;
        }
        }

        delete request;
    });
    request->execute();
}

void NetworkManager::getUpgradeJson()
{
    IRequest *request = new GetUpgradeJsonRequest(this);
    connect(request, &IRequest::requestCompleted, this, [this, request](IRequest::Status status) {
        switch (status)
        {
        case IRequest::Status::TimedOut:
        {
            qDebug() << "Request to get upgrade json timed out";
            break;
        }
        case IRequest::Status::Success:
        {
            emit upgradeInfoAvailable(m_hCasterInfo->UpgradeInfo);
            break;
        }
        case IRequest::Status::Failed:
        {
            qDebug() << "Request to get upgrade json failed";
            break;
        }
        }

        delete request;
    });
    request->execute();
}
