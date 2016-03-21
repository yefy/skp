#ifndef SKP_LOCAL_MANAGE_PARSE_H
#define SKP_LOCAL_MANAGE_PARSE_H

#include "skpObject.h"

class SkpEventChan;
class SkpLocalProtocal;
class SkpLocalSocketTcp;


class SkpLocalManageParse : public SkpObject
{
public:

    static void init();
    static void destroy();
    static SkpLocalManageParse *instance(uint64 userID);
    static std::vector<SkpLocalManageParse *> m_manages;
    static std::vector<SkpThread *> m_threads;
    ~SkpLocalManageParse();

    int m_index;
    SkpEventChan *m_chan;
    std::map<uint64, SkpLocalProtocal *> m_protocaltMap;

    bool parseChan(void *head, int headSize, void *data, int dataSize);
    bool onParseChan(void *head, int headSize, void *data, int dataSize);


private:
    SkpLocalManageParse(int index, SkpThread &thread);
};

#endif // SKP_LOCAL_MANAGE_PARSE_H
