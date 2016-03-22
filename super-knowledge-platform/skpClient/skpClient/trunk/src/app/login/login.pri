LOGIN_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${LOGIN_CURR_PATH}
DEPENDPATH  +=  \
            $${LOGIN_CURR_PATH}

HEADERS += \
    ../../src/app/login/skp_login_widget.h \
    ../../src/app/login/skp_login_main_widget.h

SOURCES += \
    ../../src/app/login/skp_login_widget.cpp \
    ../../src/app/login/skp_login_main_widget.cpp

FORMS += \
    ../../src/app/login/skp_login_main_widget.ui

