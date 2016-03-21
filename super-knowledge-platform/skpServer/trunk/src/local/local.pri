LOCAL_PATH = $${PWD}
INCLUDEPATH +=  \
            $${LOCAL_PATH}
DEPENDPATH  +=  \
            $${LOCAL_PATH}

SOURCES += \
    ../../src/local/main.cpp \
    ../../src/local/skp_local_application.cpp \
    ../../src/local/skp_local_socket_tcp.cpp \
    ../../src/local/skp_local_ack.cpp \
    ../../src/local/skp_local_chat.cpp \
    ../../src/local/skp_local_demo.cpp \
    ../../src/local/skp_local_friend.cpp \
    ../../src/local/skp_local_key.cpp \
    ../../src/local/skp_local_login.cpp \
    ../../src/local/skp_local_memory_file.cpp \
    ../../src/local/skp_local_protocal.cpp \
    ../../src/local/skp_local_status.cpp \
    ../../src/local/skp_local_tlv.cpp \
    ../../src/local/skp_local_manage_route.cpp \
    ../../src/local/skp_local_manage_parse.cpp

HEADERS += \
    ../../src/local/skp_local_application.h \
    ../../src/local/skp_local_socket_tcp.h \
    ../../src/local/skp_local_ack.h \
    ../../src/local/skp_local_chat.h \
    ../../src/local/skp_local_demo.h \
    ../../src/local/skp_local_friend.h \
    ../../src/local/skp_local_key.h \
    ../../src/local/skp_local_login.h \
    ../../src/local/skp_local_memory_file.h \
    ../../src/local/skp_local_protocal.h \
    ../../src/local/skp_local_status.h \
    ../../src/local/skp_local_tlv.h \
    ../../src/local/skp_local_manage_route.h \
    ../../src/local/skp_local_manage_parse.h
