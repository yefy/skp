CORE_PATH = $${PWD}
INCLUDEPATH +=  \
            $${CORE_PATH}
DEPENDPATH  +=  \
            $${CORE_PATH}

HEADERS += \
    ../../src/core/skpEvent.h \
    ../../src/core/skpGlobal.h \
    ../../src/core/skpInherit_test.h \
    ../../src/core/skpObject.h \
    ../../src/core/skpThread.h \
    ../../src/core/skpEventObject.h \
    ../../src/core/skpEventData.h \
    ../../src/core/skpEventTimer.h \
    ../../src/core/skpEventRead.h \
    ../../src/core/skpEventWrite.h \
    ../../src/core/skpObject_p.h \
    ../../src/core/skpThread_p.h \
    ../../src/core/skpInherit_p_test.h \
    ../../src/core/skpEvent_p.h \
    ../../src/core/skpApplication.h \
    ../../src/core/skpObjectData.h \
    ../../src/core/skpObjectData_p.h \
    ../../src/core/skpEventObject_p.h \
    ../../src/core/skpEventTimer_p.h \
    ../../src/core/skpEventData_p.h \
    ../../src/core/skpEventRead_p.h \
    ../../src/core/skpEventWrite_p.h \
    ../../src/core/skpApplication_p.h \
    ../../src/core/skpSignal.h \
    ../../src/core/skpEventChan.h \
    ../../src/core/skpEventChan_p.h \
    ../../src/core/skpSignal_p.h

SOURCES += \
    ../../src/core/skpEvent.cpp \
    ../../src/core/skpGlobal.cpp \
    ../../src/core/skpInherit_test.cpp \
    ../../src/core/skpObject.cpp \
    ../../src/core/skpThread.cpp \
    ../../src/core/skpEventObject.cpp \
    ../../src/core/skpEventTimer.cpp \
    ../../src/core/skpEventData.cpp \
    ../../src/core/skpEventRead.cpp \
    ../../src/core/skpEventWrite.cpp \
    ../../src/core/skpApplication.cpp \
    ../../src/core/skpObjectData.cpp \
    ../../src/core/skpSignal.cpp \
    ../../src/core/skpEventChan.cpp
