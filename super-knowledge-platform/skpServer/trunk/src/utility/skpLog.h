#ifndef SKPLOG_H
#define SKPLOG_H

#include "skpObjectData.h"

namespace Skp {

enum LOG_LEVEL
{
    LOG_ALL = 0,		///< Log all
    LOG_DEBUG,			///< debug message
    LOG_TRACE,			///< trace message
    LOG_ERROR,			///< error message
    LOG_FATAL,			///< fatal error message
    LOG_NONE			///< Log nothing
};

enum LOG_OUTPUT
{
    LOG_TO_NONE   = 0,	///< Log no output
    LOG_TO_FILE   = 1,	///< Log to file
    LOG_TO_STDERR = 2	///< Log to stderr
};

}


class SkpLogPrivate;

class SkpLog : public SkpObjectData
{
public:
    SkpLog(const char *pathName, const char *fileName, Skp::LOG_LEVEL logLevel, int logOutPut, bool isAddHeadTime, int maxWriteTime, int maxFileLenth, bool isThreadSafe);
    ~SkpLog();
    void create_file();
    void write_file(char *buffer);
    bool check_file();
    void log(const char *file, uint16 line, const char *function, const char *tag, Skp::LOG_LEVEL logLevel, char *buffer);
    void write_log(Skp::LOG_LEVEL logLevel, const char * format, ...);
    void write_all(const char *file, uint16 line, const char *function, const char *tag, const char * format, ...);
    void write_debug(const char *file, uint16 line, const char *function, const char *tag, const char * format, ...);
    void write_trace(const char *file, uint16 line, const char *function, const char *tag, const char * format, ...);
    void write_error(const char *file, uint16 line, const char *function, const char *tag, const char * format, ...);
    void write_fatal(const char *file, uint16 line, const char *function, const char *tag, const char * format, ...);
    static char *log_path(const char *file, uint16 line, const char *function, char *data);
private:
    SKP_DECLARE_PRIVATE(SkpLog);
};

extern SkpLog *g_log;

#define skpLogAll(log, msg, ...) if(log) log->write_all(__FILE__, __LINE__, __FUNCTION__, NULL, msg, ##__VA_ARGS__)
#define skpLogDebug(log, msg, ...) if(log) log->write_debug(__FILE__, __LINE__, __FUNCTION__, NULL, msg, ##__VA_ARGS__)
#define skpLogTrace(log, msg, ...) if(log) log->write_trace(__FILE__, __LINE__, __FUNCTION__, NULL, msg, ##__VA_ARGS__)
#define skpLogError(log, msg, ...) if(log) log->write_error(__FILE__, __LINE__, __FUNCTION__, NULL, msg, ##__VA_ARGS__)
#define skpLogFatal(log, msg, ...) if(log) log->write_fatal(__FILE__, __LINE__, __FUNCTION__, NULL, msg, ##__VA_ARGS__)

#define skpLogAll_g(msg, ...) if(g_log) g_log->write_all(__FILE__, __LINE__, __FUNCTION__, NULL, msg, ##__VA_ARGS__)
#define skpLogDebug_g(msg, ...) if(g_log) g_log->write_debug(__FILE__, __LINE__, __FUNCTION__, NULL, msg, ##__VA_ARGS__)
#define skpLogTrace_g(msg, ...) if(g_log) g_log->write_trace(__FILE__, __LINE__, __FUNCTION__, NULL, msg, ##__VA_ARGS__)
#define skpLogError_g(msg, ...) if(g_log) g_log->write_error(__FILE__, __LINE__, __FUNCTION__, NULL, msg, ##__VA_ARGS__)
#define skpLogFatal_g(msg, ...) if(g_log) g_log->write_fatal(__FILE__, __LINE__, __FUNCTION__, NULL, msg, ##__VA_ARGS__)

#endif // SKPLOG_H
