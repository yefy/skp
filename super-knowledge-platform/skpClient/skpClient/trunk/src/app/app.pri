APP_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${APP_CURR_PATH}
DEPENDPATH  +=  \
            $${APP_CURR_PATH}

HEADERS += \
    ../../src/app/skp_file_widget.h \
    ../../src/app/skp_app_utility.h \
    ../../src/app/skp_transfer_file.h

SOURCES += \
    ../../src/app/skp_file_widget.cpp \
    ../../src/app/skp_app_utility.cpp \
    ../../src/app/skp_transfer_file.cpp

FORMS += \
    ../../src/app/skp_file_widget.ui
