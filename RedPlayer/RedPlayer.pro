#-------------------------------------------------
#
# Project created by QtCreator 2017-06-30T00:41:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RedPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mediawindow.cpp

HEADERS  += mediawindow.h

FORMS    += mediawindow.ui


macx {

INCLUDEPATH += $$PWD/3rdparty/mac/libvlc/include
LIBS += -L$$PWD/3rdparty/mac/libvlc/lib -lvlc

}

win32 {
DEFINES +=MEIDA_PLAYER_WIN32
INCLUDEPATH += $$PWD/3rdparty/win32/libvlc/include
LIBS += -L$$PWD/3rdparty/win32/libvlc/lib -lvlc.dll

}
