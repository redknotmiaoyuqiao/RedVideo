TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lglfw
LIBS += -lGLEW
LIBS += -lGL

SOURCES += main.cpp \
    RedGL/GLProgram.cpp \
    RedGL/GLShader.cpp \
    RedGL/TextFile.cpp

HEADERS += \
    RedGL/RedGL.hpp \
    RedGL/File.hpp

DISTFILES += \
    GLSL/v_shader.vert \
    GLSL/f_shader.frag
