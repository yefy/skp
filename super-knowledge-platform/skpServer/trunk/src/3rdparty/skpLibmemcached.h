#ifndef SKPLIBMEMCACHED_H
#define SKPLIBMEMCACHED_H

#include "skpUtility.h"

#include "libmemcached/memcached.hpp"

class SkpLibmemcached
{
public:
    SkpLibmemcached();
    ~SkpLibmemcached();
    memcached_return appendServer(const char *ip, uint16 port);
    memcached_return start();
    memcached_return set(const char *key, size_t keySize,
                 const char *value, size_t valueSize,
                 time_t expiration, uint flags);
    char *get(const char *key, size_t keySize,
                 size_t &valueSize,
                 uint &flags, memcached_return &rc);

    memcached_return del(const char *key, size_t keySize,
                 time_t expiration);

    memcached_server_st *m_servers;
    memcached_return m_rc;
    memcached_st *m_memc;
};

#endif // SKPLIBMEMCACHED_H
