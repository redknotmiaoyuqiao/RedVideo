TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lglfw
LIBS += -lGLEW
LIBS += -lGL
LIBS += -lSOIL
LIBS += -lx264

SOURCES += main.cpp \
    RedGL/GLProgram.cpp \
    RedGL/GLShader.cpp \
    RedGL/TextFile.cpp \
    RedGL/GLTexture.cpp \
    RedGL/GLVAO.cpp \
    Camera/Camera.cpp \
    H264/H264encode.cpp

HEADERS += \
    RedGL/RedGL.hpp \
    RedGL/File.hpp \
    Camera/Camera.hpp \
    H264/H264encode.hpp

DISTFILES += \
    GLSL/v_shader.vert \
    GLSL/f_shader.frag
