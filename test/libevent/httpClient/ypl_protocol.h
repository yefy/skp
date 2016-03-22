#ifndef YPL_PROTOCOL_H
#define YPL_PROTOCOL_H

#include <map>
#include <string>

class yplProtocol
{
public:
    yplProtocol();
    void add(std::string &key, std::string &value);
    void add(const char *key, const char *value);
    void join();
    void sign();
    void uriEncoding();
    std::string &url();
    std::string &yplUrl();
private:
    char *getJoin(int size);
private:
    std::map<std::string, std::string> m_map;
    char *m_join;
    int realSize;
    int allSize;

    std::string m_url;
};

#endif // YPL_PROTOCOL_H
