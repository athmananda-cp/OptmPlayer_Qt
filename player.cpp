#include "player.h"
#include "common/player_common.h"
#include "network/player_network_manager.h"
#include "network/player_network_requests.h"
#include "common/player_utils.h"
#include <QtQuick/QQuickView>
#include <QTimer>
#include <QQmlContext>
#include <QFile>
#include <QFileInfo>
#include <QProcess>

class PlayerPrivate
{
public:
    Player                               *q {nullptr};
    QQmlApplicationEngine                *qmlApplicationEngine {nullptr};
    QSharedPointer<PlayerNetworkManager>       nwManager;
    QSharedPointer<PlayerUIManager>      swUpdateManager;
    QSharedPointer<HCasterInfo_t>        hCasterInfo;

    void registerMetaTypes()
    {
        qRegisterMetaType<VersionInfo_t>();
        qRegisterMetaType<SwUpdateInfo_t>();
        qRegisterMetaType<errorInfo_t>();		
    }

    void initApplicationEngine()
    {
        qmlApplicationEngine = new QQmlApplicationEngine(q);
        qmlRegisterUncreatableType<PlayerUIManager>("Player", 1, 0, "Player", "Should only be used as a context property");
        qmlApplicationEngine->rootContext()->setContextProperty("player", q);
    }

    void initializePlayer()
    {
        hCasterInfo = QSharedPointer<HCasterInfo_t>(new HCasterInfo_t());
        swUpdateManager = QSharedPointer<PlayerUIManager>(new PlayerUIManager(q));
        swUpdateManager->registerMetaTypes();
        nwManager = QSharedPointer<PlayerNetworkManager>(new PlayerNetworkManager(q));
    }
};

Player::Player(int &argc, char **argv)
    : QGuiApplication(argc, argv)
    , d(new PlayerPrivate)
{
    d->q = this;
    connect(this, &Player::aboutToQuit,
            this, [=]{delete d->qmlApplicationEngine;});
}

Player::~Player()
{

}

int Player::exec()
{
    d->initApplicationEngine();
    d->registerMetaTypes();
    d->initializePlayer();


    connect(d->nwManager.data(), &PlayerNetworkManager::listOfObjectsAvailable, [this]() {
        d->swUpdateManager->checkForUpdates();
    });


    connect(d->nwManager.data(), &PlayerNetworkManager::sHomeCasterStatus, [this](qint64 iStatus) {
        if(iStatus == 0)
        {
            const QUrl url = QUrl(QString("http://%1%2").arg(d->hCasterInfo->IpAddress,d->hCasterInfo->PlaybackUrl));
            d->qmlApplicationEngine->rootContext()->setContextProperty("playbackurl",url);
            d->qmlApplicationEngine->load(QUrl(QStringLiteral("qrc:/qml/startup.qml")));
            d->nwManager->getListObjects();
        }
        else
        {
            const QUrl url = QUrl(QString("http://192.168.0.108/hls/56-5003/index.m3u8"));
            d->qmlApplicationEngine->rootContext()->setContextProperty("playbackurl",url);
            d->qmlApplicationEngine->load(QUrl(QStringLiteral("qrc:/qml/startup.qml")));
            d->qmlApplicationEngine->load(QUrl(QStringLiteral("qrc:/qml/UpdateErrorDialog.qml")));
            qDebug() << "Failed to get Homecaster IP in player " ;
            d->swUpdateManager->updateError("HomeCaster Not Detected");

        }

    });

    d->nwManager->getHCasterIpAddress();

    return QGuiApplication::exec();
}

void Player::exit()
{
    QGuiApplication::exit();
}

QQmlApplicationEngine *Player::qmlApplicationEngine()
{
    return d->qmlApplicationEngine;
}

QSharedPointer<PlayerNetworkManager> Player::networkManager()
{
    return d->nwManager;
}

QSharedPointer<HCasterInfo_t> Player::hCasterInfo() const
{
    return d->hCasterInfo;
}

void Player::launchNewExe()
{
    QFile configFile("installrun.sh");
    if (! configFile.exists())
    {
        qDebug() << "Couldn't find installrun.sh";
    }
    else
    {
        QString program = "installrun.sh";
            QStringList arguments;
            arguments << QFileInfo(QCoreApplication::applicationFilePath()).fileName();

            QUrl url(d->hCasterInfo->UpgradeInfo.UpgradeImageObjectInfo.Url);
            arguments << url.fileName();

            QProcess *myProcess = new QProcess(this);
            connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                [=](int exitCode, QProcess::ExitStatus exitStatus){
                Q_UNUSED(exitCode)
                Q_UNUSED(exitStatus)
                QGuiApplication::exit();
                exit();
            });

            connect (myProcess, &QProcess::readyReadStandardError, [myProcess](){
                qDebug() << myProcess->readAllStandardOutput();
            });
            connect (myProcess, &QProcess::readyReadStandardError, [myProcess](){
                qDebug() << myProcess->readAllStandardError();
            });

            myProcess->start(program, arguments);

      //  QGuiApplication::exit();
    }
}
