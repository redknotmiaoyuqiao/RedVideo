TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lglfw
LIBS += -lGLEW
LIBS += -lGL

SOURCES += main.cpp \
    RedGL/GLProgram.cpp \
    RedGL/GLShader.cpp

HEADERS += \
    RedGL/RedGL.hpp
