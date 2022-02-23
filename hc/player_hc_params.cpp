#include "network/player_network_requests.h"
#include "player.h"
#include "network/player_network_manager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QMap>

PlayerHcGetParams::PlayerHcGetParams(QObject *parent)
    : IPlayerNetworkRequest(parent)
{
}

void PlayerHcGetParams::execute()
{
//    qDebug() << "Sending request for hcast ip";
    QNetworkRequest request(QUrl("http://hcaster.digicaps.com/get_ip.php"));
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    m_nwAccessManager->get(request);
}

void PlayerHcGetParams::onFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error())
    {
        qDebug() << "Error in receiving home caster ip address."
                 << url.toEncoded().constData() << qPrintable(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    handleResponse(data);

    reply->deleteLater();
}

void PlayerHcGetParams::handleResponse(QByteArray response)
{
    IPlayerNetworkRequest::Status retvalue = Status::Failed;
  //  qDebug() << "PlayerHcGetParams response : " << response;
    if (!response.isEmpty())
    {   
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(response, &error);
        if (error.error == QJsonParseError::NoError)
        {
            if (document.isArray())
            {
                QJsonArray ipArray = document.array();
                QVariantList ipList = ipArray.toVariantList();
                if(ipList.size()>0)
                {
                    QVariantMap ipMap = ipList.at(0).toMap();
                    qApp->hCasterInfo()->IpAddress = ipMap["client_addr"].toString();
                    retvalue =Status::Success;
                }
            }
        }
    }
    emit requestCompleted(retvalue);
}
