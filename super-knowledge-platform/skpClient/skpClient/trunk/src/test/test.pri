TEST_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${TEST_CURR_PATH}
DEPENDPATH  +=  \
            $${TEST_CURR_PATH}

SOURCES += \
    ../../src/test/skp_mainwindow_test.cpp \
    #../../src/test/main.cpp \
    ../../src/test/skp_socket_test_widget.cpp

FORMS += \
    ../../src/test/skp_mainwindow_test.ui \
    ../../src/test/skp_socket_test_widget.ui

HEADERS += \
    ../../src/test/skp_mainwindow_test.h \
    ../../src/test/skp_socket_test_widget.h
