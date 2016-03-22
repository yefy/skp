TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    rsa1.cpp \
    rsa2.cpp \
    rsa3.cpp \
    rsa4.cpp

#INCLUDEPATH += /home/yefy/skp/super-knowledge-platform/skp_server/trunk/objs/openssl-1.0.1f/local/include

#LIBS += -L/home/yefy/skp/super-knowledge-platform/skp_server/trunk/objs/openssl-1.0.1f/local/lib -lcrypto

LIBS += -lcrypto

HEADERS += \
    rsa1.h \
    rsa2.h \
    rsa3.h \
    rsa4.h
