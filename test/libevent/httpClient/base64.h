#ifndef BASE64_H
#define BASE64_H

void base64_test();

class base64
{
public:
    base64();
    static char *encode(const char * input, int length, int *outSize, bool with_new_line = false);
    static char *decode(char * input, int length, int *outSize, bool with_new_line = false);
};

#endif // BASE64_H
