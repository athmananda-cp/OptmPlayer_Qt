#ifndef PLAYER_H
#define PLAYER_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "UI/player_ui_manager.h"
#include "common/player_common.h"

#if defined (qApp)
#undef qApp
#endif
#define qApp (static_cast<Player*>(QGuiApplication::instance()))

class PlayerNetworkManager;
class PlayerPrivate;
class Player : public QGuiApplication
{
    Q_OBJECT
public:
    explicit Player(int &argc, char **argv);
    virtual ~Player() override;
    int exec();
    Q_INVOKABLE void exit();
    Q_INVOKABLE void launchNewExe();

    QQmlApplicationEngine *qmlApplicationEngine();
    QSharedPointer<PlayerNetworkManager> networkManager();
    QSharedPointer<HCasterInfo_t> hCasterInfo() const;

private:
    QScopedPointer<PlayerPrivate> d;
};

#endif // PLAYER_H
