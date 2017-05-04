TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    socketserver.c

HEADERS += \
    socketserver.h \
    msgdefine.h
