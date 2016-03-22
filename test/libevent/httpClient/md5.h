#ifndef MD5_H
#define MD5_H

#include <string>

class MD5
{
public:
    MD5();
    static std::string string(const std::string &str);
    static std::string string(const char *str);
};

#endif // MD5_H
