include(../libevent/libevent.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS  += -g -std=c++11
QMAKE_LFLAGS    += -g -std=c++11
QMAKE_CFLAGS    += -g -std=c++11

INCLUDEPATH += \
        /home/yefy/super/skp/super-knowledge-platform/skpServer/trunk/lib/rapidjson/include


LIBS    += \
        -lrt \
        -lpthread \
        -lcrypto \
        -lssl

SOURCES += \
    main.cpp \
    httpclient_ypl.cpp \
    rsa.cpp \
    base64.cpp \
    skp_rapidjson_test.cpp \
    md5.cpp \
    ypl_protocol.cpp

HEADERS += \
    httpclient_ypl.h \
    rsa.h \
    base64.h \
    skp_rapidjson_test.h \
    md5.h \
    ypl_protocol.h
