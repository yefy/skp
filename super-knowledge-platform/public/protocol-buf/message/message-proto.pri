MESSAGE_PROTO_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${MESSAGE_PROTO_CURR_PATH}
DEPENDPATH  +=  \
            $${MESSAGE_PROTO_CURR_PATH}
OTHER_FILES += \
../../../../public/protocol-buf/message/ack.proto \
../../../../public/protocol-buf/message/friend.proto \
../../../../public/protocol-buf/message/login.proto \
../../../../public/protocol-buf/message/memory_file.proto \
../../../../public/protocol-buf/message/message.proto
