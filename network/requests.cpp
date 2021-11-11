#include "requests.h"


IRequest::IRequest(QObject *parent)
    : QObject(parent)
    , m_nwAccessManager(new QNetworkAccessManager(parent))
{
    connect(m_nwAccessManager, &QNetworkAccessManager::finished, this, &IRequest::onFinished);
}

