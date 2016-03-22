#ifndef SKP_CORE_MD5_H
#define SKP_CORE_MD5_H

char* MDString(const char *string, unsigned int len);
char* MDFile(const char *);
char* hmac_md5(char* text, char* key);

#endif // SKP_CORE_MD5_H
