#-------------------------------------------------
#
# Project created by QtCreator 2013-03-23T23:19:15
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = zorbaneural

TEMPLATE = lib
#CONFIG+= staticlib


SOURCES += neuralnet.cpp

HEADERS += \
    neuralnet.h \
    neuralnet_global.h

DEFINES += ZNNLIB


symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE119305E
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = zorbaneural.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    dev.files = neuralnet.cpp neuralnet.h neuralnet_global.h
    maemo5 {
        target.path = /opt/usr/lib
        dev.path = /opt/usr/include/libzorbaneural
    } else {
        target.path = /usr/lib
        dev.path = /usr/include/libzorbaneural
    }
    INSTALLS += target dev
}
