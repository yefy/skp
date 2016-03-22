CORE_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${CORE_CURR_PATH}
DEPENDPATH  +=  \
            $${CORE_CURR_PATH}

HEADERS += \
    ../../src/gui/NcFramelessHelper.h \
    ../../src/gui/skp_title_widget.h \
    ../../src/gui/skp_toolButton.h \
    ../../src/gui/skp_utility.h \
    ../../src/gui/skp_change_widget.h \
    ../../src/gui/skp_tabBar.h \
    ../../src/gui/skp_text_tabBar.h \
    ../../src/gui/skp_themed_base.h \
    ../../src/gui/skp_custom_dialog.h \
    ../../src/gui/skp_skin_themed_widget.h \
    ../../src/gui/MSkinChanger.h \
    ../../src/gui/RecSkinDialog.h \
    ../../src/gui/RecSkinPickerWidget.h \
    ../../src/gui/RecSkinWidget.h \
    ../../src/gui/MOption.h \
    ../../src/gui/QjtMessageBox.h

SOURCES += \
    ../../src/gui/NcFramelessHelper.cpp \
    ../../src/gui/skp_title_widget.cpp \
    ../../src/gui/skp_toolButton.cpp \
    ../../src/gui/skp_utility.cpp \
    ../../src/gui/skp_change_widget.cpp \
    ../../src/gui/skp_tabBar.cpp \
    ../../src/gui/skp_text_tabBar.cpp \
    ../../src/gui/skp_themed_base.cpp \
    ../../src/gui/skp_custom_dialog.cpp \
    ../../src/gui/skp_skin_themed_widget.cpp \
    ../../src/gui/MSkinChanger.cpp \
    ../../src/gui/RecSkinDialog.cpp \
    ../../src/gui/RecSkinPickerWidget.cpp \
    ../../src/gui/RecSkinWidget.cpp \
    ../../src/gui/MOption.cpp \
    ../../src/gui/QjtMessageBox.cpp

FORMS += \
    ../../src/gui/skp_title_widget.ui \
    ../../src/gui/skp_tabBar.ui \
    ../../src/gui/skp_custom_dialog.ui \
    ../../src/gui/MSkinChanger.ui \
    ../../src/gui/RecSkinDialog.ui \
    ../../src/gui/RecSkinPickerWidget.ui \
    ../../src/gui/RecSkinWidget.ui
