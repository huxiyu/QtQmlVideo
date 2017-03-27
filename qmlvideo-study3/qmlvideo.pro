#include(qmldeploy.pri)

QT += qml quick widgets core opengl

CONFIG += c++11

SOURCES += main.cpp \
    qmlvideo.cpp

RESOURCES += \
    qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(qmldeploy.pri)

HEADERS += \
    qmlvideo.h


    INCLUDEPATH += "D:/gitrep/github/QtQmlVideo/Lib/glew-1.9.0/include"
    LIBS += D:/gitrep/github/QtQmlVideo/Lib/glew-1.9.0/lib/glew32.lib
    INCLUDEPATH += "D:/Test/Lib/VLC/sdk/include"
    LIBS += D:/gitrep/github/QtQmlVideo/Lib/VLC/sdk/lib/libvlccore.lib
    LIBS += D:/gitrep/github/QtQmlVideo/Lib/VLC/sdk/lib/libvlc.lib

    LIBS += opengl32.lib







