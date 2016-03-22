#-------------------------------------------------
#
# Project created by QtCreator 2015-01-29T16:30:17
#
#-------------------------------------------------

include(../../src/main/main.pri)
include(../../src/app/app.pri)
include(../../src/core/core.pri)
include(../../src/gui/gui.pri)
include(../../src/test/test.pri)
include(../../src/resources/resources.pri)
include(../../../../public/public.pri)
include(../../../../public/protocol-buf/message.pri)
include(../../../../public/protocol-buf/message/message-proto.pri)
include(../../src/app/login/login.pri)
include(../../src/app/chat/chat.pri)

QT       += core gui network sql

DESTDIR += ../skp-client/bin

win32::QT += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = skp-client
TEMPLATE = app

win32:RC_FILE = ../../src/resources/skp.rc

QMAKE_CXXFLAGS  += -std=c++11
QMAKE_LFLAGS    += -std=c++11
QMAKE_CFLAGS    += -std=c++11

INCLUDEPATH   += \
C:\MinGW\msys\1.0\home\Administrator\protobuf-master\build-mingw\include

LIBS          += \
C:\MinGW\msys\1.0\home\Administrator\protobuf-master\build-mingw\lib\libprotobuf.a \

