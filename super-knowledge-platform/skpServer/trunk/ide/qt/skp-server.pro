include(../../src/utility/utility.pri)
include(../../src/core/core.pri)
include(../../src/3rdparty/3rdparty.pri)
include(../../src/network/network.pri)
include(../../src/local/local.pri)
#include(../../src/test/test.pri)

include(../../../../public/public.pri)
include(../../../../public/protocol-buf/message.pri)
include(../../../../public/protocol-buf/message/message-proto.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR += ./bin

#git add -u .
#git commit -m "updata"
#git push


#cd /home/yefy/skp/super-knowledge-platform/skp_server/trunk/objs/memcached-1.4.22/
#memcached -d -m 10 -p 11211 -u nobody

#perf stat ./skp-server
#perf record -e cpu-clock ./skp-server
#perf report


#GPROF = -pg -lc_p
#GPROF_MAIN = -pg
#gprof skp-server gmon.out -p
#gprof skp-server gmon.out -q
#gprof -b ./skp-server gmon.out > ./t.log
#../../../lib/gprof/gprof2dot.py ./t.log | dot -Tpng -o ./out.png


GPERF_MAIN = -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free -ansi
#export PPROF_PATH=../../../lib/gperftools/bin/pprof

#rm -f ./skp.prof* && env CPUPROFILE=./skp.prof ./skp-server
#../../../lib/gperftools/bin/pprof --text skp-server ./skp.prof* > profile.txt
#../../../lib/gperftools/bin/pprof --pdf ./skp-server ./skp.prof* > profile.pdf

#rm -f skp.*.heap && env HEAPPROFILE=./skp ./skp-server
#../../../lib/gperftools/bin/pprof --text skp-server ./*.heap > profile.txt
#../../../lib/gperftools/bin/pprof --pdf skp-server ./*.heap > profile.pdf

#env HEAPCHECK=normal ./skp-server
#env PPROF_PATH=../../../lib/gperftools/bin/pprof HEAPCHECK=normal ./skp-server


#有的时候gmc显示符号有问题，无法显示函数，那么就直接运行pprof，gmc会有提示，譬如：
#pprof ./objs/srs "/tmp/srs.11469._main_-end.heap" --inuse_objects --lines --heapcheck  --edgefraction=1e-10 --nodefraction=1e-10 --gv
#需要改动两个地方：
#pprof改成./objs/pprof。
#去掉--gv，直接进入命令行，然后输入top就可以看到。

#../../../lib/gperftools/bin/pprof ./skp "/tmp/skp.5413._main_-end.heap" --inuse_objects --lines --heapcheck  --edgefraction=1e-10 --nodefraction=1e-10

#../../../lib/gperftools/bin/pprof ./skp-server "/tmp/skp-server.4639._main_-end.heap" --inuse_objects --lines --heapcheck  --edgefraction=1e-10 --nodefraction=1e-10


QMAKE_CXXFLAGS  += -g -O0 -std=c++11 $${GPROF}
QMAKE_LFLAGS    += -g -O0 -std=c++11 $${GPROF_MAIN} $${GPERF_MAIN}
QMAKE_CFLAGS    += -g -O0 -std=c++11 $${GPROF}


unix::INCLUDEPATH   += \
    ../../lib/gperftools/include \
    ../../lib/libmemcached/include \
    ../../lib/protobuf/include \
    ../../lib/hiredis/include \
    ../../lib/gmock/include \
    ../../lib/gmock/gtest/include \
    /usr/local/mysql/include

unix::LIBS          += \
    ../../lib/gperftools/lib/libtcmalloc_and_profiler.a \
    ../../lib/libmemcached/lib/libmemcached.a \
    ../../lib/protobuf/lib/libprotobuf.a \
    ../../lib/hiredis/lib/libhiredis.a \
    ../../lib/gmock/lib/libgmock.a \
    ../../lib/gmock/gtest/lib/libgtest.a \
    /usr/local/mysql/lib/libmysqlclient.a \
    /usr/lib/libz.a \
    -lrt \
    -lsasl2 \
    -lpthread

