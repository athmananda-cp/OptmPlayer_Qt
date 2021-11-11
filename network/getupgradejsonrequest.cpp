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

#ifdef DUMMY_SERVER_DATA
#define RESPONSE \
"{" \
  "\"imageName\": \"OptmPlayer_2.12.3_28-Oct-2021_01-53-05.apk\"," \
  "\"majorVersion\": 1, "\
  "\"minorVersion\": 2, "\
  "\"listOfChanges\": [ "\
    "\"1. Lag time reduced to 0.2 milliseconds\n\", \"2. WiFi connectivity improved\n\", \"3. Minor bugs have been rectified\n\", \"4. Visual enhancements made\n\", \"5. Various performance improvements\" "\
  "]" \
"}"
#endif

GetUpgradeJsonRequest::GetUpgradeJsonRequest(QObject *parent)
    : IRequest(parent)
{

}

void GetUpgradeJsonRequest::execute()
{
    for (auto objInfo : qApp->networkManager()->hCasterInfo()->ObjectInfoList)
    {
        if (objInfo.AppContextIdList != "UpgradeImage")
            continue;

        if (objInfo.Url.endsWith("upgrade.json"))
        {
            qDebug() << "Upgrade json URL : " << objInfo.Url;
#ifdef DUMMY_SERVER_DATA
            QByteArray data = QString(RESPONSE).toUtf8();
            handleResponse(data);
#else
            QNetworkRequest request(QUrl(objInfo.Url));
            m_nwAccessManager->get(request);
#endif
            return;
        }
    }

    emit requestCompleted(Status::Failed);
}

void GetUpgradeJsonRequest::onFinished(QNetworkReply *reply)
{
    // Process reply
    QUrl url = reply->url();
    if (reply->error())
    {
        qDebug() << "Error in GetUpgradeJsonRequest "
                 << url.toEncoded().constData()
                 << qPrintable(reply->errorString());

        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    handleResponse(data);
    reply->deleteLater();
}

void GetUpgradeJsonRequest::handleResponse(QByteArray response)
{
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(response, &error);
    if (error.error != QJsonParseError::NoError)
    {
        qDebug() << "Error string : " << error.errorString();
        emit requestCompleted(Status::Failed);
        return;
    }

    // Find the object info for given image and update the upgrade info
    QJsonObject jObjRoot = document.object();
    for (auto infoObj : qApp->networkManager()->hCasterInfo()->ObjectInfoList)
    {
        if (infoObj.Url.endsWith(jObjRoot["imageName"].toString()))
        {
            qApp->networkManager()->hCasterInfo()->UpgradeInfo.UpgradeImageObjectInfo = infoObj;
            qApp->networkManager()->hCasterInfo()->UpgradeInfo.MajorVersion = jObjRoot["majorVersion"].toInt();
            qApp->networkManager()->hCasterInfo()->UpgradeInfo.MinorVersion = jObjRoot["minorVersion"].toInt();
            QJsonArray jArrListOfChanges = jObjRoot["listOfChanges"].toArray();
            foreach (const QJsonValue & value, jArrListOfChanges)
                qApp->networkManager()->hCasterInfo()->UpgradeInfo.ListOfChanges.append(value.toString());

            emit requestCompleted(Status::Success);
            return;
        }
    }

    emit requestCompleted(Status::Failed);
}

