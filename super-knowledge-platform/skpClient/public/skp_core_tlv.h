#ifndef SKP_CORE_TLV_H
#define SKP_CORE_TLV_H

#include "assert.h"

#define SKP_ASSERT(x)   assert(x);


#define SKP_TRUE    true
#define SKP_FALSE   false

#define SKP_NULL    NULL


/*  signed  */
//typedef bool            int1;
//typedef char		int8;
typedef short int		int16;
//typedef int			int32;
typedef long long int		int64;

/*  unsigned    */
//typedef unsigned char		uint8;
typedef unsigned short int	uint16;
//typedef unsigned int		uint32;
typedef unsigned long long int	uint64;

typedef unsigned int		uintptr;

typedef unsigned char		uchar;
typedef unsigned short int	uint16;
typedef unsigned int		uint;
typedef unsigned long long int	uint64;

/*
 * system use   0x0000   0x03E0
 */
const uint16 _Tag_System_Min    = 0x0000;
const uint16 _Tag_System_Test   = 0x0001;

const uint16 _Tag_System_Max    = 0x03E0;

/*
 * app use  0x03E1
 */
const uint16 _Tag_Type      = 0x03E1;
const uint16 _Tag_Number    = 0x03E2;
const uint16 _Tag_CurrSzie  = 0x03E3;
const uint16 _Tag_AllSize   = 0x03E4;
const uint16 _Tag_Local     = 0x03E5;
const uint16 _Tag_Target    = 0x03E6;
const uint16 _Tag_MD5       = 0x03E7;
const uint16 _Tag_Data      = 0x03E8;
const uint16 _Tag_Send_Time = 0x03E9;
const uint16 _Tag_Recv_Time = 0x03EA;
const uint16 _Tag_UserID    = 0x03EB;
const uint16 _Tag_ServerID  = 0x03EC;
const uint16 _Tag_Password  = 0x03ED;




typedef struct
{
    uchar nHead:1;
    uchar nSmall:7;  //move 0   & 0x7f
    uchar nBig;      //move 7   & 0xff
}TagType;

typedef struct
{
    uchar nHead:2;
    uchar nOne:6;   //move 0    & 0x3f
    uchar nTwo;     //move 6    & 0xff
    uchar nThree;   //move 14   & 0xff
    uchar nFour;    //move 22   & 0xff
}LengthType;

namespace SKP_TLV {
    enum TAG_TYPE
    {
        TAG_TYPE_SMALL = 0,
        TAG_TYPE_BIG
    };

    /*
     0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15

     0 TAG_TYPE

     1 -> 7  TAG_TYPE_SMALL  127

     8 -> 15 TAG_TYPE_BIG    32767

     */

    enum LENGTH_TYPE
    {
        LENGTH_TYPE_ONE = 0,
        LENGTH_TYPE_TWO,
        LENGTH_TYPE_THREE,
        LENGTH_TYPE_FOUR
    };

    /*
     0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15

     0 -> 1 LENGTH_TYPE

     2 -> 7  LENGTH_TYPE_ONE   63

     8 -> 15 LENGTH_TYPE_TWO

     16 -> 23 LENGTH_TYPE_THREE

     24 -> 31 LENGTH_TYPE_FOUR

     */
}

class SkpTLV
{
public:
    SkpTLV();
    SkpTLV(char *pBuffer, int64 nSize);
    SkpTLV(char *pBuffer, int64 nSize, int tlvSize, int headSize);
    virtual ~SkpTLV();
    void init(char *pBuffer, int64 nSize, int tlvSize, int headSize);
    void reset();
    void reset(int size);
    void realloc(int nsize);
    char *set_Head(int nSize);
    char *Head();
    bool is_Head();
    void *byte();
    void *byte_tlv();
    uint64 use_size();
    uint64 tlv_size();
    void copy(char *pBuffer, int tlvSize, int headSize);
    void copyTLV(char *pBuffer, int tlvSize);
    int appen(int size);
    int page(int nSize);

    int add(uint16 nTag, const void *pValue, int nLength);
    int add_null(uint16 nTag,int nLength);
    char *seek_value();
    int seek(uint16 nTag);
    int next();

    int add_char(uint16 nTag, char nValue);
    int add_uchar(uint16 nTag, uchar nValue);

    int add_short(uint16 nTag, int16 nValue);
    int add_ushort(uint16 nTag, uint16 nValue);

    int add_int(uint16 nTag, int nValue);
    int add_uint(uint16 nTag, uint nValue);

    int add_large_int(uint16 nTag, int64 nValue);
    int add_large_uint(uint16 nTag, uint64 nValue);

    int add_double(uint16 nTag, double nValue);

    char seek_char(uint16 nTag);
    uchar seek_uchar(uint16 nTag);

    int16 seek_short(uint16 nTag);
    uint16 seek_ushort(uint16 nTag);

    int seek_int(uint16 nTag);
    uint seek_uint(uint16 nTag);

    int64 seek_large_int(uint16 nTag);
    uint64 seek_large_uint(uint16 nTag);

    double seek_double(uint16 nTag);

//private:
    int tag_encode(TagType *pTagType, uint16 nTag);
    int tag_decode(TagType *pTagType, uint16 &nTag);
    int length_encode(LengthType *pLengthType, uint nLength);
    int length_decode(LengthType *pLengthType, uint &nLength);



private:
    char *m_pBuffer;
    uint64 m_nBufferSize;
    uint64 m_nBufferUseSize;
    uint64 m_nBufferSpareSize;
    char *m_pBufferUseTail;
    char *m_pSeekTag;
    char *m_pSeekLength;
    char *m_pSeekValue;
    uint m_nSeekValueSize;
    bool m_isSelf;
    char *m_pHead;
    int m_nHeadSize;
};


#endif // SKP_CORE_TLV_H
