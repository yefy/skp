#include "skpLibmemcached.h"

SkpLibmemcached::SkpLibmemcached() :
    m_servers(NULL)
{
    m_memc = memcached_create(NULL);

}

SkpLibmemcached::~SkpLibmemcached()
{
    if(m_servers) {
        memcached_server_free(m_servers);
        m_servers = NULL;
    }

    if(m_memc) {
        memcached_free(m_memc);
        m_memc = NULL;
    }

}

memcached_return SkpLibmemcached::appendServer(const char *ip, uint16 port)
{
    m_servers = memcached_server_list_append(m_servers, ip, port, &m_rc);
    return m_rc;
}

memcached_return SkpLibmemcached::start()
{
    return memcached_server_push(m_memc, m_servers);
}

memcached_return SkpLibmemcached::set(const char *key, size_t keySize,
                              const char *value, size_t valueSize,
                              time_t expiration, uint flags)
{
    return memcached_set(m_memc, key, keySize, value, valueSize, expiration, flags);
}

char *SkpLibmemcached::get(const char *key, size_t keySize,
             size_t &valueSize,
             uint &flags, memcached_return &rc)
{
    return memcached_get(m_memc, key, keySize, &valueSize, &flags, &rc);
}

memcached_return SkpLibmemcached::del(const char *key, size_t keySize,
             time_t expiration)
{
    return memcached_delete(m_memc, key, keySize, expiration);
}
