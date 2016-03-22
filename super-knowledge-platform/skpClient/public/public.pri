PUBLIC_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${PUBLIC_CURR_PATH}
DEPENDPATH  +=  \
            $${PUBLIC_CURR_PATH}

HEADERS += \
    ../../../../public/skp_public_utility.h \
    ../../../../public/skp_public_ins.h \
    ../../../../public/skp_public_message_class.h \
    ../../../../public/skp_public_transfer_protocal.h \
    ../../../../public/skp_core_md5.h \
    ../../../../public/skp_core_tlv.h \
    ../../../../public/skp_local_tlv.h

SOURCES += \
    ../../../../public/skp_public_utility.cpp \
    ../../../../public/skp_public_ins.cpp \
    ../../../../public/skp_public_message_class.cpp \
    ../../../../public/skp_public_transfer_protocal.cpp \
    ../../../../public/skp_core_md5.cpp \
    ../../../../public/skp_core_tlv.cpp \
    ../../../../public/skp_local_tlv.cpp


