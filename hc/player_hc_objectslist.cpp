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

//#define ID_RESULTS "result"
//#define ID_LIST_OF_OBJECTS "listOfObjects"
//#define ID_URL "Url"
//#define ID_APP_CONTEXT_ID_LIST "appContextIdList"
//#define ID_CONTEXT_LENGTH "contentLength"
//#define ID_CONTENT_LOCATION "contentLocation"
//#define ID_CONTENT_TYPE "contentType"
//#define ID_DOWNLOAD_STATUS "downloadStatus"
//#define ID_EXPIRES "expires"
//#define ID_NO_OF_BYTES_RECEIVED "numberOfBytesReceived"
//#define ID_RECEIVED_TIME "receivedTime"
//#define ID_TRANSFER_LENGTH "transferLength"

PlayerHcGetObjectList::PlayerHcGetObjectList(QObject *parent)
    : IPlayerNetworkRequest(parent)
{

}

void PlayerHcGetObjectList::execute()
{
    const QUrl url = QUrl(QString("http://%1:81/jsonrpc").arg(qApp->hCasterInfo()->IpAddress));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["jsonrpc"] = "2.0";
    obj["method"]  = "com.digicap.atsc.listObjects";

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    m_nwAccessManager->post(request, data);
}

void PlayerHcGetObjectList::onFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error())
    {
        qDebug() << "Error in PlayerHcGetObjectList "
                 << url.toEncoded().constData()
                 << qPrintable(reply->errorString());

        reply->deleteLater();
        return;
    }
    QByteArray data = reply->readAll();
    handleResponse(data);
    reply->deleteLater();
}

void PlayerHcGetObjectList::handleResponse(QByteArray response)
{
    if (response.isEmpty())
    {
        qDebug() << "Failed to get object list. Response is empty.";
        emit requestCompleted(Status::Failed);
        return;
    }

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(response, &error);
    if (error.error != QJsonParseError::NoError)
    {
        qDebug() << "Error string : " << error.errorString();
        emit requestCompleted(Status::Failed);
        return;
    }

    QJsonObject jObjResult = document.object()["result"].toObject();
    QJsonArray jArrListOfObjects = jObjResult["listOfObjects"].toArray();
    foreach (const QJsonValue & value, jArrListOfObjects)
    {
        QJsonObject obj = value.toObject();
        QString appContextIdList = obj.value("appContextIdList").toString();
        if (appContextIdList == "UpgradeImage")
        {
            ObjectInfo_t info = {
                obj.value("Url").toString()
                , obj.value("appContextIdList").toString()
                , obj.value("contentLength").toInt()
                , obj.value("contentLocation").toString()
                , obj.value("contentType").toString()
                , obj.value("downloadStatus").toString()
                , obj.value("expires").toInt()
                , obj.value("numberOfBytesReceived").toInt()
                , obj.value("receivedTime").toInt()
                , obj.value("transferLength").toInt()
            };
            qApp->hCasterInfo()->ObjectInfoList.append(info);
        }
    }
    emit requestCompleted(Status::Success);
}

