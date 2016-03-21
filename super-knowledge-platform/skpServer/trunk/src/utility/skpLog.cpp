#include "skpLog.h"
#include "skpMutex.h"
#include "skpMalloc.h"

#include "skpObjectData_p.h"

class SkpLogPrivate : public SkpObjectDataPrivate
{
public:
  SkpLogPrivate();
  virtual ~SkpLogPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpLog);

private:
  char *m_pathName;
  char *m_fileName;
  char *m_completeName;
  FILE *m_fp;
  Skp::LOG_LEVEL m_logLevel;
  int m_logOutPut;
  int m_isAddHeadTime;
  int m_maxWriteTime;
  int m_maxFileLenth;
  time_t m_createTime;
  bool m_isThreadSafe;
  SkpMutex m_fileMutex;
  int m_error;
};

SkpLogPrivate::SkpLogPrivate() :
    SkpObjectDataPrivate()
{

}

SkpLogPrivate::~SkpLogPrivate()
{

}


//============================SkpLog

#define LOG_ERROR_ALL(msg, ...)   { skpD->m_error++; if(skpD->m_error > 10) return; skpLogError_g(msg, ##__VA_ARGS__); }

static const char * strLogLevel[] =
{
    "LOG_ALL",
    "LOG_DEBUG",
    "LOG_TRACE",
    "LOG_ERROR",
    "LOG_FATAL",
    "LOG_NONE"
};


SkpLog::SkpLog(const char *pathName, const char *fileName,
               Skp::LOG_LEVEL logLevel, int logOutPut,
               bool isAddHeadTime, int maxWriteTime,
               int maxFileLenth, bool isThreadSafe) :
    SkpObjectData(*new SkpLogPrivate())
{
    SKP_ASSERT(pathName);
    SKP_ASSERT(fileName);
    SKP_ASSERT(maxWriteTime >= 0);
    SKP_ASSERT(maxFileLenth >= 0);


    SKP_D(SkpLog);

    skpD->m_fp = 0;
    int size = strlen((const char *)pathName) + 1;

    skpD->m_pathName = (char *)skp_malloc(size);
    memset(skpD->m_pathName, 0x00, size);
    strcpy((char *)skpD->m_pathName, pathName);

    size = strlen(fileName) + 1;

    skpD->m_fileName = (char *)skp_malloc(size);
    memset(skpD->m_fileName,0x00, size);
    strcpy(skpD->m_fileName, fileName);

    skpD->m_completeName = (char *)skp_malloc(4096);
    memset(skpD->m_completeName, 0x00, 4096);

    SKP_ASSERT(logLevel >= Skp::LOG_ALL && logLevel <= Skp::LOG_NONE);
    skpD->m_logLevel = logLevel;

    skpD->m_logOutPut = logOutPut;
    skpD->m_isAddHeadTime = isAddHeadTime;
    skpD->m_maxWriteTime = maxWriteTime;
    skpD->m_maxFileLenth = maxFileLenth;
    skpD->m_isThreadSafe = isThreadSafe;

    int isFile = skpD->m_logOutPut & Skp::LOG_TO_FILE;
    if(isFile)
        create_file();

}

SkpLog::~SkpLog()
{
    SKP_D(SkpLog);

    skp_free(skpD->m_pathName);
    skp_free(skpD->m_fileName);
    skp_free(skpD->m_completeName);
    if(skpD->m_fp) {
        fclose(skpD->m_fp);
        skpD->m_fp = 0;
    }
}

void SkpLog::create_file()
{
    SKP_D(SkpLog);

    struct timeval *tv = skp_get_system_time_tv();

    time_t tt = tv->tv_sec;
    tm *p = localtime(&tt);
    snprintf(skpD->m_completeName, 4095, "%s%s%04u%02u%02u%02u%02u%02u%03ld.log",
            skpD->m_pathName?skpD->m_pathName:"",skpD->m_fileName,
            p->tm_year + 1900, p->tm_mon + 1, p->tm_mday,
            p->tm_hour, p->tm_min, p->tm_sec,tv->tv_usec / 1000);

    skpD->m_fp = fopen(skpD->m_completeName, "w+");

    if(!skpD->m_fp)
    {
        LOG_ERROR_ALL("file fopen error\n");
    }

    skpD->m_createTime = tt;
}

void SkpLog::log(const char *file, uint16 line, const char *function, const char *tag, Skp::LOG_LEVEL logLevel, char *buffer)
{
    SKP_D(SkpLog);

    SKP_ASSERT(logLevel >= Skp::LOG_ALL && logLevel <= Skp::LOG_NONE);
    SKP_ASSERT(buffer);

    if(logLevel < skpD->m_logLevel)
        return;

    char position[4096];
    memset(position,0x00,sizeof(position));
    int size = 0;
    if(file)
    {
        size += sprintf(position, "%s ", file);
    }

    if(line > 0)
    {
        size += sprintf(position + size, "%d ", line);
    }

    if(function)
    {
        size += sprintf(position + size, "%s ", function);
    }

    if(tag)
    {
        size += sprintf(position + size, "%s ", tag);
    }

    struct timeval *tv = skp_get_system_time_tv();

    time_t second = tv->tv_sec;
    tm *tm    = localtime(&second);

    char prefix[4096];
    memset(prefix,0x00,sizeof(prefix));
    if (skpD->m_isAddHeadTime)
    {
        sprintf(prefix, "[%04d-%02d-%02d %02d:%02d:%02d.%03ld %s %s ] ",
                tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
                tm->tm_hour, tm->tm_min, tm->tm_sec,	tv->tv_usec / 1000,
                strLogLevel[logLevel], position);
    }

    if(!skpD->m_logOutPut)
        return;

    char logBuffer[4096];
    memset(logBuffer,0x00,sizeof(logBuffer));
    sprintf(logBuffer,"%s%s",prefix,buffer);

    if (skpD->m_logOutPut & Skp::LOG_TO_STDERR)
    {
        //printf("[%02d:%02d:%02d.%03ld %s %s] %s",
        //       tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec / 1000,
        //       m_fileName, position, buffer);
        printf(logBuffer);
    }

    if(skpD->m_logOutPut & Skp::LOG_TO_FILE)
    {
        write_file(logBuffer);
    }
}
void SkpLog::write_file(char *buffer)
{
    SKP_D(SkpLog);

    SKP_ASSERT(buffer);


    // Check if file exists

    if (access(skpD->m_completeName, F_OK) < 0)
    {
        LOG_ERROR_ALL("file not exists\n");
        clearerr(skpD->m_fp);
        fclose(skpD->m_fp);
        skpD->m_fp = skp_null;

        // Try to create another file
        create_file();
        if(skpD->m_fp == skp_null)
            return;
    }

    if(skpD->m_fp)
    {
        char size = strlen(buffer);
        fwrite(buffer, 1, size, skpD->m_fp);
        fflush(skpD->m_fp);
    }

    // Check if error occurs
    if (ferror(skpD->m_fp))
    {
        LOG_ERROR_ALL("file ferror\n");
        clearerr(skpD->m_fp);
        fclose(skpD->m_fp);
        skpD->m_fp = skp_null;


        // Try to create another file
        create_file();
        if(skpD->m_fp == skp_null)
            return;

        // Write again
        if(skpD->m_fp)
        {
            char size = strlen(buffer);
            fwrite(buffer, 1, size, skpD->m_fp);
            fflush(skpD->m_fp);
        }

        if (ferror(skpD->m_fp))
        {
            LOG_ERROR_ALL("file ferror2\n");
            clearerr(skpD->m_fp);
            fclose(skpD->m_fp);
            skpD->m_fp = skp_null;
            return;
        }
    }

    /// Split files
    if(skpD->m_maxWriteTime > 0 || skpD->m_maxFileLenth > 0)
    {
        // Check if log file is needed to be changed
        if (!check_file())
        {

            clearerr(skpD->m_fp);
            fclose(skpD->m_fp);
            skpD->m_fp = skp_null;


            // Try to create another file
            create_file();
            if(skpD->m_fp == skp_null)
                return;
        }
    }
}


bool SkpLog::check_file()
{
    SKP_D(SkpLog);

    if (skp_null == skpD->m_fp)
        return skp_false;

    // Check if file exists
    if (access(skpD->m_completeName, F_OK) < 0)
        return skp_false;

    // Check the size
    long len = ftell(skpD->m_fp);
    if (len < 0)
        return skp_false;

    if (skpD->m_maxFileLenth && len >= skpD->m_maxFileLenth)
        return skp_false;

    // Check the time
    time_t currTime = time(NULL);

    if (skpD->m_maxWriteTime && difftime(currTime,skpD->m_createTime) > skpD->m_maxWriteTime)
        return skp_false;


    return skp_true;
}

void SkpLog::write_log(Skp::LOG_LEVEL logLevel, const char * format, ...)
{
    SKP_D(SkpLog);

    if(logLevel < skpD->m_logLevel)
        return;
    if(skpD->m_isThreadSafe) {
        __SkpAutoLocker(skpD->m_fileMutex);
    }
    char logmsg[4096];
    memset(logmsg, 0, sizeof(logmsg));

    va_list args;
    va_start(args, format);

#ifdef _WIN32
    _vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#else
    vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#endif

    va_end(args);
    log(skp_null, 0, skp_null, skp_null, logLevel,logmsg);
}

void SkpLog::write_all(const char *file, uint16 line, const char *function, const char *tag, const char * format, ...)
{
    SKP_D(SkpLog);

    if(Skp::LOG_ALL < skpD->m_logLevel)
        return;
    if(skpD->m_isThreadSafe) {
        __SkpAutoLocker(skpD->m_fileMutex);
    }
    char logmsg[4096];
    memset(logmsg, 0, sizeof(logmsg));

    va_list args;
    va_start(args, format);
//_WIN32 //__linux
#ifdef _WIN32
    _vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#else
    vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#endif

    va_end(args);
    log(file, line, function, tag, Skp::LOG_ALL,logmsg);
}

void SkpLog::write_debug(const char *file, uint16 line, const char *function, const char *tag, const char * format, ...)
{
    SKP_D(SkpLog);

    if(Skp::LOG_DEBUG < skpD->m_logLevel)
        return;
    if(skpD->m_isThreadSafe) {
        __SkpAutoLocker(skpD->m_fileMutex);
    }
    char logmsg[4096];
    memset(logmsg, 0, sizeof(logmsg));

    va_list args;
    va_start(args, format);

#ifdef _WIN32
    _vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#else
    vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#endif

    va_end(args);
    log(file, line, function, tag, Skp::LOG_DEBUG,logmsg);
}

void SkpLog::write_trace(const char *file, uint16 line, const char *function, const char *tag, const char * format, ...)
{
    SKP_D(SkpLog);

    if(Skp::LOG_TRACE < skpD->m_logLevel)
        return;
    if(skpD->m_isThreadSafe) {
        __SkpAutoLocker(skpD->m_fileMutex);
    }
    char logmsg[4096];
    memset(logmsg, 0, sizeof(logmsg));

    va_list args;
    va_start(args, format);

#ifdef _WIN32
    _vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#else
    vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#endif

    va_end(args);
    log(file, line, function, tag, Skp::LOG_TRACE,logmsg);
}

void SkpLog::write_error(const char *file, uint16 line, const char *function, const char *tag, const char * format, ...)
{
    SKP_D(SkpLog);

    if(Skp::LOG_ERROR < skpD->m_logLevel)
        return;
    if(skpD->m_isThreadSafe) {
        __SkpAutoLocker(skpD->m_fileMutex);
    }
    char logmsg[4096];
    memset(logmsg, 0, sizeof(logmsg));

    va_list args;
    va_start(args, format);

#ifdef _WIN32
    _vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#else
    vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#endif

    va_end(args);
    log(file, line, function, tag, Skp::LOG_ERROR,logmsg);
}

void SkpLog::write_fatal(const char *file, uint16 line, const char *function, const char *tag, const char * format, ...)
{
    SKP_D(SkpLog);

    if(Skp::LOG_FATAL < skpD->m_logLevel)
        return;
    if(skpD->m_isThreadSafe) {
        __SkpAutoLocker(skpD->m_fileMutex);
    }
    char logmsg[4096];
    memset(logmsg, 0, sizeof(logmsg));

    va_list args;
    va_start(args, format);

#ifdef _WIN32
    _vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#else
    vsnprintf(logmsg, sizeof(logmsg) - 1, format, args);
#endif

    va_end(args);
    log(file, line, function, tag, Skp::LOG_FATAL,logmsg);
}

char *SkpLog::log_path(const char *file, uint16 line, const char *function, char *data)
{
    int size = 0;
    if(file)
    {
        size += sprintf(data, "%s ", file);
    }

    if(line > 0)
    {
        size += sprintf(data + size, "%d ", line);
    }

    if(function)
    {
        size += sprintf(data + size, "%s ", function);
    }

    return data;
}
