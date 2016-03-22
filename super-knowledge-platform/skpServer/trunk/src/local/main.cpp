#include "skp_local_application.h"
#include "skpLog.h"
#include "gmock/gmock.h"
//#include <gperftools/profiler.h>
//#include <gperftools/heap-profiler.h>

SkpLog *g_log = skp_null;

int main(int argc, char *argv[])
{
    //ProfilerStart(NULL);
    //HeapProfilerStart(NULL);

    g_log = new SkpLog("", "main_server_", Skp::LOG_ERROR, /*Skp::LOG_TO_FILE | */Skp::LOG_TO_STDERR,skp_true,0,1024 * 1024 *100, skp_true);

//    testing::InitGoogleMock(&argc, argv);
//    return RUN_ALL_TESTS();

    SkpLocalApplication app(argc, argv);
    int res = app.exec();

    //ProfilerStop();
    //HeapProfilerStop();
    return res;
}
