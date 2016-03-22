MAIN_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${MAIN_CURR_PATH}
DEPENDPATH  +=  \
            $${MAIN_CURR_PATH}


SOURCES +=  $${MAIN_CURR_PATH}/skp_mainwindow.cpp \
            $${MAIN_CURR_PATH}/main.cpp

HEADERS  += $${MAIN_CURR_PATH}/skp_mainwindow.h

FORMS    += $${MAIN_CURR_PATH}/skp_mainwindow.ui
