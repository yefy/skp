CORE_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${CORE_CURR_PATH}
DEPENDPATH  +=  \
            $${CORE_CURR_PATH}

HEADERS += \
    ../../src/core/skp_tcp_socket.h \
    ../../src/core/skp_socket_manage.h \
    ../../src/core/skp_core_manage.h \
    ../../src/core/skp_core_signal.h

SOURCES += \
    ../../src/core/skp_tcp_socket.cpp \
    ../../src/core/skp_socket_manage.cpp \
    ../../src/core/skp_core_manage.cpp \
    ../../src/core/skp_core_signal.cpp
