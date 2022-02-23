QT += quick network multimedia 

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#DEFINES += DUMMY_SERVER_DATA

SOURCES += \
        UI/viewmodels/player_ui_view_fwdownload.cpp \
        UI/viewmodels/player_ui_view_fwinfo.cpp \
        UI/viewmodels/player_ui_view_fwinstall.cpp \
        common/player_common.cpp \
        main.cpp \
        UI/player_ui_data.cpp \
        hc/player_hc_upgradeimage.cpp \
        hc/player_hc_params.cpp \
        hc/player_hc_objectslist.cpp \
	hc/player_hc_servicelist.cpp \
        hc/player_hc_upgradejson.cpp \
        network/player_network_manager.cpp \
        network/player_network_requests.cpp \
        network/player_network_responses.cpp \
        player.cpp \
        UI/player_ui_manager.cpp \
        common/player_utils.cpp

HEADERS += \
    UI/viewmodels/player_ui_view_fwdownload.h \
    UI/viewmodels/player_ui_view_fwinfo.h \
    UI/viewmodels/player_ui_view_fwinstall.h \
    common/player_common.h \
    UI/player_ui_data.h \
    network/player_network_manager.h \
    network/player_network_requests.h \
    network/player_network_responses.h \
    player.h \
    UI/player_ui_manager.h \
    common/player_utils.h

DISTFILES += \
    config/versioninfo.ini
    scripts/installrun.sh
RESOURCES += UI/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

LIBS       += -L/usr/local/lib/ -lVLCQtCore -lVLCQtWidgets -lVLCQtQml
INCLUDEPATH += /usr/local/lib/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#TEMPLATE = lib
#CONFIG += staticlib
