QT += quick network

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#DEFINES += DUMMY_SERVER_DATA

SOURCES += \
        UI/viewmodels/swupdatedownloadviewmodel.cpp \
        UI/viewmodels/swupdateinfoviewmodel.cpp \
        UI/viewmodels/swupdateinstallviewmodel.cpp \
        commontypes.cpp \
        main.cpp \
        UI/models/swupdatedatamodel.cpp \
        network/getbinaryimagerequest.cpp \
        network/gethomecasteripaddressrequest.cpp \
        network/getlistobjectsrequest.cpp \
        network/getupgradejsonrequest.cpp \
        network/networkmanager.cpp \
        network/requests.cpp \
        network/responses.cpp \
        player.cpp \
        UI/swupdatemanager.cpp \
        utils.cpp

HEADERS += \
    UI/viewmodels/swupdatedownloadviewmodel.h \
    UI/viewmodels/swupdateinfoviewmodel.h \
    UI/viewmodels/swupdateinstallviewmodel.h \
    commontypes.h \
    UI/models/swupdatedatamodel.h \
    network/networkmanager.h \
    network/requests.h \
    network/responses.h \
    player.h \
    UI/swupdatemanager.h \
    utils.h

DISTFILES += \
    config/versioninfo.ini

RESOURCES += UI/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

