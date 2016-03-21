#include "skp_test_application.h"
#include "skpLog.h"

SkpLog *g_log = skp_null;

int main(int argc, char *argv[])
{
    g_log = new SkpLog("", "main_server_", Skp::LOG_DEBUG, /*Skp::LOG_TO_FILE | */Skp::LOG_TO_STDERR,true,0,1024 * 1024 *100, true);

    SkpTestApplication app(argc, argv);
    return app.exec();
}
