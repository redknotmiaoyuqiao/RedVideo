TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

macx
{
    INCLUDEPATH += ../libs/GLFW/include
    INCLUDEPATH += ../libs/GLEW/include

    LIBS += -L ../libs/GLFW/lib
    LIBS += -L ../libs/GLEW/lib
}
SOURCES += main.cpp
