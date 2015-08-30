#package needed: g++-multilib

#QMAKESPEC=linux-g++-32

#QMAKESPEC=linux-g++-64

#QT += core\
# widgets \
#svg \
#    script
QT += widgets \
      script \
      printsupport

TARGET = kartesio
TEMPLATE = app

FORMS += \
    mainwindow.ui

OTHER_FILES += \
    kartesio.desktop
    #libzorbaneural/main.cpp \

HEADERS += \
    calculations.h \
    mainwindow.h \
    ui_mainwindow.h \
    libzorbaneural/neuralnet.h \
    libzorbaneural/neuralnet_global.h \
    qcustomplot/qcustomplot.h

SOURCES += \
    calculations.cpp \
    main.cpp \
    mainwindow.cpp \
    libzorbaneural/neuralnet.cpp \
    qcustomplot/qcustomplot.cpp

RESOURCES     = application.qrc
