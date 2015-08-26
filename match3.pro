TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    modellist.cpp \
    item.cpp \
    gameconfig.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    modellist.h \
    item.h \
    gameconfig.h

CONFIG += c++11
