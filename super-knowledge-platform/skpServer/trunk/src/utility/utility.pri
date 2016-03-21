UTILITY_PATH = $${PWD}
INCLUDEPATH +=  \
            $${UTILITY_PATH}
DEPENDPATH  +=  \
            $${UTILITY_PATH}

HEADERS += \
    ../../src/utility/skpUtility.h \
    ../../src/utility/skpAutoFree.h \
    ../../src/utility/skpRbtree.h \
    ../../src/utility/skpQueue.h \
    ../../src/utility/skpList.h \
    ../../src/utility/skpLog.h \
    ../../src/utility/skpMalloc.h \
    ../../src/utility/skpMutex.h \
    ../../src/utility/skpMallocPool.h \
    ../../src/utility/skpMallocPoolEx.h \
    ../../src/utility/skpListEx.h \
    ../../src/utility/skpMD5.h \
    ../../src/utility/skpBuffer.h \
    ../../src/utility/skpTcpStream.h \
    ../../src/utility/skpPipe.h \
    ../../src/utility/skpTlv.h

SOURCES += \
    ../../src/utility/skpUtility.cpp \
    ../../src/utility/skpAutoFree.cpp \
    ../../src/utility/skpRbtree.cpp \
    ../../src/utility/skpQueue.cpp \
    ../../src/utility/skpList.cpp \
    ../../src/utility/skpLog.cpp \
    ../../src/utility/skpMalloc.cpp \
    ../../src/utility/skpMutex.cpp \
    ../../src/utility/skpMallocPool.cpp \
    ../../src/utility/skpMallocPoolEx.cpp \
    ../../src/utility/skpListEx.cpp \
    ../../src/utility/skpMD5.cpp \
    ../../src/utility/skpBuffer.cpp \
    ../../src/utility/skpTcpStream.cpp \
    ../../src/utility/skpPipe.cpp \
    ../../src/utility/skpTlv.cpp

