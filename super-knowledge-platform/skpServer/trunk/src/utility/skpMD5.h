#ifndef SKPMD5_H
#define SKPMD5_H


class SkpMD5
{
public:
    SkpMD5();
    ~SkpMD5();
    static char *string(char *string, unsigned int len);
    static char *file(char *string);
    static char *mac(char* text, char* key);
};

#endif // SKPMD5_H
