CHAT_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${CHAT_CURR_PATH}
DEPENDPATH  +=  \
            $${CHAT_CURR_PATH}

HEADERS += \
    ../../src/app/chat/skp_chat_widget.h \
    ../../src/app/chat/skp_chat_main_widget.h \
    ../../src/app/chat/skp_transfer_file_widget.h \
    ../../src/app/chat/skp_transfer_file_item_widget.h \
    ../../src/app/chat/ColoredProgressBar.h \
    ../../src/app/chat/skp_text_edit.h

SOURCES += \
    ../../src/app/chat/skp_chat_widget.cpp \
    ../../src/app/chat/skp_chat_main_widget.cpp \
    ../../src/app/chat/skp_transfer_file_widget.cpp \
    ../../src/app/chat/skp_transfer_file_item_widget.cpp \
    ../../src/app/chat/ColoredProgressBar.cpp \
    ../../src/app/chat/skp_text_edit.cpp

FORMS += \
    ../../src/app/chat/skp_chat_main_widget.ui \
    ../../src/app/chat/skp_transfer_file_widget.ui \
    ../../src/app/chat/skp_transfer_file_item_widget.ui