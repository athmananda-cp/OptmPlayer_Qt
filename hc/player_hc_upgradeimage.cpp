#include "network/player_network_requests.h"
#include "player.h"
#include "network/player_network_manager.h"

#include <QFile>
#include <QDataStream>

PlayerHcGetUpgradeImage::PlayerHcGetUpgradeImage(QObject *parent)
    : IPlayerNetworkRequest(parent)
{
}

void PlayerHcGetUpgradeImage::execute()
{
    UpgradeInfo_t upgradeInfoObject = qApp->hCasterInfo()->UpgradeInfo;
    QNetworkRequest request(QUrl(upgradeInfoObject.UpgradeImageObjectInfo.Url));

    qDebug() << "Downloading binary : " << upgradeInfoObject.UpgradeImageObjectInfo.Url;
    QNetworkReply *reply = m_nwAccessManager->get(request);
    connect(reply, &QNetworkReply::downloadProgress, this, &PlayerHcGetUpgradeImage::downloadProgress);
}

void PlayerHcGetUpgradeImage::onFinished(QNetworkReply *reply)
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

void PlayerHcGetUpgradeImage::handleResponse(QByteArray response)
{
    qDebug() << "PlayerHcGetUpgradeImage::handleResponse";
    if (response.isEmpty())
    {
        qDebug() << "Failed to get binary image. Response is empty.";
        emit requestCompleted(Status::Failed);
        return;
    }

    UpgradeInfo_t upgradeInfoObject = qApp->hCasterInfo()->UpgradeInfo;
    QUrl url(upgradeInfoObject.UpgradeImageObjectInfo.Url);
    QFile file("/home/savi/project/" + url.fileName());
    file.open(QIODevice::WriteOnly);
    file.write(response);
    file.flush();
    file.close();

    emit requestCompleted(Status::Success);
}
