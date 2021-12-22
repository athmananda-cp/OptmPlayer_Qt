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

    QCoreApplication::setApplicationName("OptmPlayer");
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

//    QGuiApplication app(argc, argv);
//    VlcCommon::setPluginPath(app.applicationDirPath() + "/plugins");
    VlcQmlVideoPlayer::registerPlugin();

    Player(argc,argv).exec();

}
