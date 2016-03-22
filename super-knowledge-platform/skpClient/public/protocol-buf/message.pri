MESSAGE_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${MESSAGE_CURR_PATH}
DEPENDPATH  +=  \
            $${MESSAGE_CURR_PATH}
HEADERS += \
../../../../public/protocol-buf/ack.pb.h \
../../../../public/protocol-buf/friend.pb.h \
../../../../public/protocol-buf/login.pb.h \
../../../../public/protocol-buf/memory_file.pb.h \
../../../../public/protocol-buf/message.pb.h
SOURCES += \
../../../../public/protocol-buf/ack.pb.cc \
../../../../public/protocol-buf/friend.pb.cc \
../../../../public/protocol-buf/login.pb.cc \
../../../../public/protocol-buf/memory_file.pb.cc \
../../../../public/protocol-buf/message.pb.cc
