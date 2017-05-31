TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

macx
{
    INCLUDEPATH += ../maclibs/GL/include
    LIBS += -L ../maclibs/GL/lib

    LIBS += -lglfw3
    LIBS += -lGLEW
    #GLFW 依赖于 Cocoa
    LIBS += -framework Cocoa
    LIBS += -framework IOKit
    LIBS += -framework CoreVideo
}
SOURCES += main.cpp

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -mmacosx-version-min=10.12
QMAKE_LFLAGS += -mmacosx-version-min=10.12
