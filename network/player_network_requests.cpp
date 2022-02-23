#include "player_network_requests.h"


IPlayerNetworkRequest::IPlayerNetworkRequest(QObject *parent)
    : QObject(parent)
    , m_nwAccessManager(new QNetworkAccessManager(parent))
{
    connect(m_nwAccessManager, &QNetworkAccessManager::finished, this, &IPlayerNetworkRequest::onFinished);
}

