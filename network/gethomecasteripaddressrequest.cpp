#include "requests.h"
#include "player.h"
#include "network/networkmanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QMap>

GetHomeCasterIpAddressRequest::GetHomeCasterIpAddressRequest(QObject *parent)
    : IRequest(parent)
{
}

void GetHomeCasterIpAddressRequest::execute()
{
    qDebug() << "Sending request for hcast ip";
    QNetworkRequest request(QUrl("http://hcaster.digicaps.com/get_ip.php"));
    m_nwAccessManager->get(request);
}

void GetHomeCasterIpAddressRequest::onFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error())
    {
        qDebug() << "Error in receiving home caster ip address."
                 << url.toEncoded().constData() << qPrintable(reply->errorString());
        reply->deleteLater();
        return;
    }

    // Process reply
#ifdef DUMMY_SERVER_DATA
    QByteArray data = QString("[{\"client_addr\":\"192.168.0.108\"}]").toUtf8();
    handleResponse(data);
#else
    QByteArray data = reply->readAll();
    handleResponse(data);
#endif
    reply->deleteLater();
}

void GetHomeCasterIpAddressRequest::handleResponse(QByteArray response)
{
    qDebug() << "GetHomeCasterIpAddressRequest response : " << response;

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(response, &error);
    if (error.error != QJsonParseError::NoError)
    {
        qDebug() << "Error string : " << error.errorString();
        emit requestCompleted(Status::Failed);
        return;
    }

    if (! document.isArray())
    {
        qDebug() << "Invalid json";
        emit requestCompleted(Status::Failed);
        return;
    }
    QJsonArray ipArray = document.array();
    QVariantList ipList = ipArray.toVariantList();
    QVariantMap ipMap = ipList.at(0).toMap();
    qApp->networkManager()->hCasterInfo()->IpAddress = ipMap["client_addr"].toString();

    emit requestCompleted(Status::Success);
}
