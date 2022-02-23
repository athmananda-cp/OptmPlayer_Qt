#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>

#include <VLCQtCore/Common.h>
#include <VLCQtQml/QmlVideoPlayer.h>
#include "player.h"
#include <VLCQtCore/Common.h>
#include <VLCQtQml/QmlVideoPlayer.h>

int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QCoreApplication::setApplicationName("OptmPlayerQt");
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

    VlcQmlVideoPlayer::registerPlugin();

    Player(argc,argv).exec();

}
