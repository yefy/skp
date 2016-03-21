TEST_PATH = $${PWD}
INCLUDEPATH +=  \
            $${TEST_PATH}
DEPENDPATH  +=  \
            $${TEST_PATH}

HEADERS += \
    ../../src/test/skp_test_application.h \
    ../../src/test/skp_test_socket_tcp.h

SOURCES += \
    ../../src/test/main.cpp \
    ../../src/test/skp_test_application.cpp \
    ../../src/test/skp_test_socket_tcp.cpp


