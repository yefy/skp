include(../libevent/libevent.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#INCLUDEPATH += \
#    ../../objs/libevent/include


LIBS    += \
        #../../objs/libevent/lib/*.a \
        -lrt \
        -lpthread

SOURCES += main.c \
    skp_libevent_test.c \
    skp_libevent_buffer_test.c \
    skp_libevent_listen.c \
    time-test.c \
    dns-example.c \
    event-test.c \
    hello-world.c \
    http-server.c \
    signal-test.c



HEADERS += \
    skp_libevent_test.h \
    skp_libevent_buffer_test.h \
    skp_libevent_listen.h \
    http-server.h

