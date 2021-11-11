#include "player.h"
#include "commontypes.h"
#include "network/networkmanager.h"
#include "network/requests.h"

#include <QTimer>
#include <QQmlContext>

class PlayerPrivate
{
public:
    Player                               *q {nullptr};
    QQmlApplicationEngine                *qmlApplicationEngine {nullptr};
    QSharedPointer<NetworkManager>       nwManager;
    QSharedPointer<SwUpdateManager>      swUpdateManager;

    void registerMetaTypes()
    {
        qRegisterMetaType<VersionInfo_t>();
        qRegisterMetaType<SwUpdateInfo_t>();
    }

    void initApplicationEngine()
    {
        qmlApplicationEngine = new QQmlApplicationEngine(q);

        qmlRegisterUncreatableType<SwUpdateManager>("Player", 1, 0, "Player", "Should only be used as a context property");
        qmlApplicationEngine->rootContext()->setContextProperty("player", q);
    }

    void initializePlayer()
    {
        swUpdateManager = QSharedPointer<SwUpdateManager>(new SwUpdateManager(q));
        swUpdateManager->registerMetaTypes();

        nwManager = QSharedPointer<NetworkManager>(new NetworkManager(q));
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
    d->qmlApplicationEngine->load(QUrl(QStringLiteral("qrc:/qml/startup.qml")));

    connect(d->nwManager.data(), &NetworkManager::listOfObjectsAvailable, [this]() {
        d->swUpdateManager->checkForUpdates();
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

QSharedPointer<NetworkManager> Player::networkManager()
{
    return d->nwManager;
}
