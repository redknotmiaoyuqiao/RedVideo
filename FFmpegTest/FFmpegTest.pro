TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += $$PWD/../macFFmpegLibs/include
LIBS += -L$$PWD/../macFFmpegLibs/lib
LIBS += -lavcodec   \
        -lavdevice  \
        -lavfilter  \
        -lavformat  \
        -lavutil    \
        -lswscale
