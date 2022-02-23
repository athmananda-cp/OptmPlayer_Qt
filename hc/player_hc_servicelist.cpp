#include "network/player_network_requests.h"
#include "player.h"
#include "network/player_network_manager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpPart>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QMap>


PlayerHcGetServiceList::PlayerHcGetServiceList(QObject *parent)
    : IPlayerNetworkRequest(parent)
{

}

void PlayerHcGetServiceList::execute()
{
    const QUrl Url = QUrl(QString("http://%1/msg/guide.json").arg(qApp->hCasterInfo()->IpAddress));
//    qDebug() << "service json URL : " << Url;

    QNetworkRequest request(Url);
    m_nwAccessManager->get(request);
}

void PlayerHcGetServiceList::onFinished(QNetworkReply *reply)
{
    // Process reply
    QUrl url = reply->url();
    if (reply->error())
    {
        qDebug() << "Error in PlayerHcGetServiceList "
                 << url.toEncoded().constData()
                 << qPrintable(reply->errorString());

        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    handleResponse(data);
    reply->deleteLater();
}

void PlayerHcGetServiceList::handleResponse(QByteArray response)
{
    IPlayerNetworkRequest::Status retvalue = Status::Failed;
    if (!response.isEmpty())
    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(response, &error);
        if (error.error == QJsonParseError::NoError)
        {
            retvalue = Status::Success;

            if ((! (document.isNull())) && (document.isObject()))
            {

                QJsonObject json_obj = document.object();
                QJsonArray guideArray = json_obj["guide"].toArray();
                QJsonObject obj = guideArray[0].toObject();
                QJsonArray serviceArray = obj["services"].toArray();
                foreach (const QJsonValue & service, serviceArray)
                {
                    QJsonObject obj = service.toObject();
//                    qDebug() << "PlayerHcGetServiceList hls :" << obj["hls"];
                    if(!(obj["hls"].isNull()))
                    {
                        qApp->hCasterInfo()->PlaybackUrl = obj["hls"].toString();
                         break;
                    }
                    else
                    {
//                        qDebug() << "PlayerHcGetServiceList hls is null data :";
                    }
                }
            }

        }
     }

    emit requestCompleted(retvalue);

}

