NETWORK_PATH = $${PWD}
INCLUDEPATH +=  \
            $${NETWORK_PATH}
DEPENDPATH  +=  \
            $${NETWORK_PATH}

HEADERS += \
    ../../src/network/skpSocket.h \
    ../../src/network/skpSocket_p.h \
    ../../src/network/skpSocketTcp.h \
    ../../src/network/skpSocketTcp_p.h \
    ../../src/network/skpServerTcp.h \
    ../../src/network/skpServerTcp_p.h

SOURCES += \
    ../../src/network/skpSocket.cpp \
    ../../src/network/skpSocketTcp.cpp \
    ../../src/network/skpServerTcp.cpp
