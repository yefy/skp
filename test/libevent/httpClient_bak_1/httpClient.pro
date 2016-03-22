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

SOURCES += \
    main.cpp \
    httpclient_ypl.cpp

HEADERS += \
    httpclient_ypl.h
