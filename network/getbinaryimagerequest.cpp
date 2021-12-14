#include "requests.h"
#include "player.h"
#include "network/networkmanager.h"

#include <QFile>
#include <QDataStream>

GetBinaryImageRequest::GetBinaryImageRequest(QObject *parent)
    : IRequest(parent)
{
}

void GetBinaryImageRequest::execute()
{
    UpgradeInfo_t upgradeInfoObject = qApp->hCasterInfo()->UpgradeInfo;
    QNetworkRequest request(QUrl(upgradeInfoObject.UpgradeImageObjectInfo.Url));

    qDebug() << "Downloading binary : " << upgradeInfoObject.UpgradeImageObjectInfo.Url;
    QNetworkReply *reply = m_nwAccessManager->get(request);
    connect(reply, &QNetworkReply::downloadProgress, this, &GetBinaryImageRequest::downloadProgress);
}

void GetBinaryImageRequest::onFinished(QNetworkReply *reply)
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
#else
    QByteArray data = reply->readAll();
    handleResponse(data);
#endif
    reply->deleteLater();
}

void GetBinaryImageRequest::handleResponse(QByteArray response)
{
    qDebug() << "GetBinaryImageRequest::handleResponse";
    if (response.isEmpty())
    {
        qDebug() << "Failed to get binary image. Response is empty.";
        emit requestCompleted(Status::Failed);
        return;
    }

    UpgradeInfo_t upgradeInfoObject = qApp->hCasterInfo()->UpgradeInfo;
    QUrl url(upgradeInfoObject.UpgradeImageObjectInfo.Url);
    QFile file("/tmp/" + url.fileName());
    file.open(QIODevice::WriteOnly);
    file.write(response);
    file.flush();
    file.close();

    emit requestCompleted(Status::Success);
}
