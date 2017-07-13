TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lx264

SOURCES += main.cpp \
    Camera/Camera.cpp \
    H264/H264Encoder.cpp

HEADERS += \
    Camera/Camera.hpp \
    H264/H264Encoder.hpp
