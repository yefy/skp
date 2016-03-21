PUBLIC_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${PUBLIC_CURR_PATH}
DEPENDPATH  +=  \
            $${PUBLIC_CURR_PATH}

HEADERS += \
    ../../../../public/skp_public_utility.h \
    ../../../../public/skp_public_ins.h \
    ../../../../public/skp_public_message_class.h \
    ../../../../public/skp_public_transfer_protocal.h

SOURCES += \
    ../../../../public/skp_public_utility.cpp \
    ../../../../public/skp_public_ins.cpp \
    ../../../../public/skp_public_message_class.cpp \
    ../../../../public/skp_public_transfer_protocal.cpp


