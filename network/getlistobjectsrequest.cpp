#include "requests.h"
#include "player.h"
#include "networkmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpPart>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QMap>

#ifdef DUMMY_SERVER_DATA
#define RESPONSE \
"{" \
    "\"id\" : 35," \
    "\"jsonrpc\" : \"2.0\"," \
    "\"result\" : " \
    "{" \
        "\"listOfObjects\" : " \
        "[" \
            "{"\
                "\"Url\" : \"http://192.168.1.114/files/56-5001/upgrade\"," \
                "\"appContextIdList\" : \"UpgradeImage\"," \
                "\"contentLength\" : 17966193," \
                "\"contentLocation\" : \"upgrade\"," \
                "\"contentType\" : \"multipart/related\"," \
                "\"downloadStatus\" : \"done\", "\
                "\"expires\" : 4294967295," \
                "\"numberOfBytesReceived\" : 17966193," \
                "\"receivedTime\" : 1636111453, "\
                "\"transferLength\" : 17966193 "\
            "}," \
            "{" \
                "\"Url\" : \"http://192.168.1.114/files/56-5001/upgrade-files/ envelope.xml\", " \
                "\"appContextIdList\" : \"UpgradeImage\", " \
                "\"contentLength\" : 314, " \
                "\"contentLocation\" : \" envelope.xml\", " \
                "\"contentType\" : \" application/mbms-envelope+xml\", " \
                "\"downloadStatus\" : \"done\", " \
                "\"expires\" : 4294967295, " \
                "\"numberOfBytesReceived\" : 314, " \
                "\"receivedTime\" : 1636111453, " \
                "\"transferLength\" : 314 " \
            "}," \
            "{" \
                "\"Url\" : \"http://192.168.1.114/files/56-5001/upgrade-files/OptmPlayer_2.12.3_28-Oct-2021_01-53-05.apk\", " \
                "\"appContextIdList\" : \"UpgradeImage\", "\
                "\"contentLength\" : 17965399, " \
                "\"contentLocation\" : \"OptmPlayer_2.12.3_28-Oct-2021_01-53-05.apk\", " \
                "\"contentType\" : \"text/plain\", " \
                "\"downloadStatus\" : \"done\", " \
                "\"expires\" : 4294967295, " \
                "\"numberOfBytesReceived\" : 17965399, " \
                "\"receivedTime\" : 1636111453, " \
                "\"transferLength\" : 17965399 " \
            "}," \
            "{" \
                "\"Url\" : \"http://192.168.1.114/files/56-5001/upgrade-files/upgrade.json\", " \
                "\"appContextIdList\" : \"UpgradeImage\", " \
                "\"contentLength\" : 49, " \
                "\"contentLocation\" : \"upgrade.json\", " \
                "\"contentType\" : \"application/json\", " \
                "\"downloadStatus\" : \"done\", " \
                "\"expires\" : 4294967295, " \
                "\"numberOfBytesReceived\" : 49, " \
                "\"receivedTime\" : 1636111453, " \
                "\"transferLength\" : 49 " \
            "}" \
        "]," \
        "\"numberOfObjects\" : 4 " \
    "}" \
"}"
#endif

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

GetListObjectsRequest::GetListObjectsRequest(QObject *parent)
    : IRequest(parent)
{

}

void GetListObjectsRequest::execute()
{
    const QUrl url = QUrl(QString("http://%1:81/jsonrpc").arg(qApp->networkManager()->hCasterInfo()->IpAddress));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["jsonrpc"] = "2.0";
    obj["method"] = "com.digicap.atsc.listObjects";

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    m_nwAccessManager->post(request, data);
}

void GetListObjectsRequest::onFinished(QNetworkReply *reply)
{
    // Process reply
#ifdef DUMMY_SERVER_DATA
    Q_UNUSED(reply)
    QByteArray data = QString(RESPONSE).toUtf8();
    handleResponse(data);
#else
    QUrl url = reply->url();
    if (reply->error())
    {
        qDebug() << "Error in GetListObjectsRequest "
                 << url.toEncoded().constData()
                 << qPrintable(reply->errorString());

        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    handleResponse(data);
    reply->deleteLater();
#endif
}

void GetListObjectsRequest::handleResponse(QByteArray response)
{
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
            qApp->networkManager()->hCasterInfo()->ObjectInfoList.append(info);
        }
    }

    emit requestCompleted(Status::Success);
}

