#ifndef PLAYER_H
#define PLAYER_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "UI/swupdatemanager.h"

#if defined (qApp)
#undef qApp
#endif
#define qApp (static_cast<Player*>(QGuiApplication::instance()))

class NetworkManager;
class PlayerPrivate;
class Player : public QGuiApplication
{
    Q_OBJECT
public:
    explicit Player(int &argc, char **argv);
    virtual ~Player() override;
    int exec();
    Q_INVOKABLE void exit();
    QQmlApplicationEngine *qmlApplicationEngine();
    QSharedPointer<NetworkManager> networkManager();

private:
    QScopedPointer<PlayerPrivate> d;
};

#endif // PLAYER_H
