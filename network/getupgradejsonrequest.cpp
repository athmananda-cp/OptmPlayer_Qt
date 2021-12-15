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
    for (auto objInfo : qApp->hCasterInfo()->ObjectInfoList)
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
    if (response.isEmpty())
    {
        qDebug() << "Failed to get upgrade json. Response is empty.";
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

    // Find the object info for given image and update the upgrade info
    if (! document.isArray())
    {
        qDebug() << "Error : Expected json array ... not found.";
        emit requestCompleted(Status::Failed);
        return;
    }
    QJsonObject jObjRoot;
    foreach (const QJsonValue & v, document.array())
    {
        QJsonObject jsonObject = v.toObject();
        if (jsonObject["platform"].toString() == "linux")
        {
            jObjRoot = jsonObject;
            break;
        }
    }

    if (jObjRoot.isEmpty())
    {
        qDebug() << "Error : Could not find object for platform linux.";
        emit requestCompleted(Status::Failed);
        return;
    }

    for (auto infoObj : qApp->hCasterInfo()->ObjectInfoList)
    {
        if (jObjRoot["platform"].toString() == "linux" && infoObj.Url.endsWith(jObjRoot["imageName"].toString()))
        {
            qApp->hCasterInfo()->UpgradeInfo.UpgradeImageObjectInfo = infoObj;
            QString strVersionInfo = jObjRoot["Version"].toString();
            QStringList versionNumbers = strVersionInfo.split(".");
        qDebug() << "versionNumbers.length() " <<  versionNumbers.length() << strVersionInfo;
            if (versionNumbers.length() >= 3)
            {
                qApp->hCasterInfo()->UpgradeInfo.MajorVersion        = versionNumbers[0].toInt();
                qApp->hCasterInfo()->UpgradeInfo.MinorVersion       = versionNumbers[1].toInt();
                qApp->hCasterInfo()->UpgradeInfo.VersionNumber  = versionNumbers[2].toInt();

                QJsonArray jArrListOfChanges = jObjRoot["listOfChanges"].toArray();
                foreach (const QJsonValue & value, jArrListOfChanges)
                    qApp->hCasterInfo()->UpgradeInfo.ListOfChanges.append(value.toString());

                emit requestCompleted(Status::Success);
                return;
            }
        }
    }

    emit requestCompleted(Status::Failed);
}

