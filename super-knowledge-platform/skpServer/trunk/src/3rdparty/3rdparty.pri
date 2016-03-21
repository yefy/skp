3RDPARTY_PATH = $${PWD}
INCLUDEPATH +=  \
            $${3RDPARTY_PATH}
DEPENDPATH  +=  \
            $${3RDPARTY_PATH}

HEADERS += \
    ../../src/3rdparty/skp_mysql_conn.h \
    ../../src/3rdparty/skpHiredis.h \
    ../../src/3rdparty/skpLibmemcached.h

SOURCES += \
    ../../src/3rdparty/skp_mysql_conn.cpp \
    ../../src/3rdparty/skpHiredis.cpp \
    ../../src/3rdparty/skpLibmemcached.cpp

