#include "skpTlv.h"
#include "skpMalloc.h"

static int g_nPageSize = 4 * 1024;

SkpTLV::SkpTLV()
{
    init(skp_null, 0, 0, 0);
}

SkpTLV::SkpTLV(char *pBuffer, int64 nSize)
{
    if(pBuffer)
    {
        SKP_ASSERT(nSize > 0);
        m_isSelf = skp_false;
    }
    else
    {
        SKP_ASSERT(nSize >= 0);
        nSize = page(nSize);

        pBuffer = (char *)skp_malloc(nSize);
        m_isSelf = skp_true;
    }
    m_pBuffer = pBuffer;
    SKP_ASSERT(m_pBuffer);
    m_nBufferSize = nSize;
    reset();

}

SkpTLV::SkpTLV(char *pBuffer, int64 nSize, int tlvSize, int headSize)
{
    init(pBuffer, nSize, tlvSize, headSize);
}

SkpTLV::~SkpTLV()
{
    if(m_isSelf)
    {
        skp_free(m_pBuffer);
        m_nBufferSize = 0;
    }
}

void SkpTLV::init(char *pBuffer, int64 nSize, int tlvSize, int headSize)
{
    //SKP_ASSERT(pBuffer);
    //SKP_ASSERT(nSize);

    m_isSelf = skp_false;

    m_pBuffer = pBuffer;
    m_nBufferSize = nSize;

    m_nBufferUseSize = tlvSize + headSize;
    m_nBufferSpareSize = m_nBufferSize - m_nBufferUseSize;
    m_pBufferUseTail = m_pBuffer + m_nBufferUseSize;
    m_pSeekTag = skp_null;
    m_pSeekLength = skp_null;
    m_pSeekValue = skp_null;
    m_nSeekValueSize = 0;
    m_pHead = m_pBuffer;
    m_nHeadSize = headSize;
}

void SkpTLV::reset()
{
    m_nBufferUseSize = 0;
    m_nBufferSpareSize = m_nBufferSize - m_nBufferUseSize;
    m_pBufferUseTail = m_pBuffer;
    m_pSeekTag = skp_null;
    m_pSeekLength = skp_null;
    m_pSeekValue = skp_null;
    m_nSeekValueSize = 0;
    m_pHead = skp_null;
    m_nHeadSize = 0;
}
void SkpTLV::reset(int size)
{
    reset();
    set_Head(size);
}

void SkpTLV::realloc(int nSize)
{
    SKP_ASSERT(m_isSelf);
    SKP_ASSERT(m_pBuffer);

    int nReallocSize = 2 * (nSize + m_nBufferUseSize);

    m_nBufferSize = page(nReallocSize);
    m_pBuffer = (char *)::realloc(m_pBuffer, m_nBufferSize);
    m_nBufferSpareSize = m_nBufferSize - m_nBufferUseSize;
    m_pBufferUseTail = m_pBuffer + m_nBufferUseSize;
    m_pSeekTag = skp_null;
    m_pSeekLength = skp_null;
    m_pSeekValue = skp_null;
    m_nSeekValueSize = 0;
    if(m_nHeadSize > 0)
        m_pHead = m_pBuffer;

}

char *SkpTLV::set_Head(int nSize)
{
    SKP_ASSERT(nSize > 0);
    SKP_ASSERT(m_pBuffer);
    SKP_ASSERT(m_nBufferUseSize == 0);
    if(m_isSelf)
    {
        if((uint64)nSize > m_nBufferSpareSize)
        {
            realloc(nSize);
        }
    }
    else
    {
        SKP_ASSERT((uint64)nSize <= m_nBufferSpareSize);
    }

    m_nBufferUseSize = nSize;
    m_nBufferSpareSize = m_nBufferSize - m_nBufferUseSize;
    m_pBufferUseTail = m_pBuffer + nSize;
    m_pSeekTag = skp_null;
    m_pSeekLength = skp_null;
    m_pSeekValue = skp_null;
    m_nSeekValueSize = 0;
    m_pHead = m_pBuffer;
    m_nHeadSize = nSize;
    skp_memzero(m_pHead, m_nHeadSize);

    return m_pHead;
}

char *SkpTLV::Head()
{
    return m_pHead;
}

bool SkpTLV::is_Head()
{
    return (m_pHead?skp_true:skp_false);
}

void *SkpTLV::byte()
{
    return m_pBuffer;
}

void *SkpTLV::byte_tlv()
{
    if(is_Head())
        return m_pBuffer +  m_nHeadSize;
    return m_pBuffer;
}

uint64 SkpTLV::use_size()
{
    return m_nBufferUseSize;
}

uint64 SkpTLV::tlv_size()
{
    return m_nBufferUseSize - m_nHeadSize;
}

void SkpTLV::copy(char *pBuffer, int tlvSize, int headSize)
{
    if(m_nBufferUseSize > 0)
        SKP_ASSERT(headSize == 0);
    int size = tlvSize + headSize;
    if((uint64)size > m_nBufferSpareSize)
        realloc(size);

    memcpy(m_pBuffer + m_nBufferUseSize, pBuffer, size);
    m_nBufferUseSize += size;
    m_nBufferSpareSize = m_nBufferSize - m_nBufferUseSize;
    m_pBufferUseTail = m_pBuffer + m_nBufferUseSize;
    m_pSeekTag = skp_null;
    m_pSeekLength = skp_null;
    m_pSeekValue = skp_null;
    m_nSeekValueSize = 0;
    if(headSize) {
        m_pHead = m_pBuffer;
        m_nHeadSize = headSize;
    }

}

void SkpTLV::copyTLV(char *pBuffer, int size)
{
    if((uint64)size > m_nBufferSpareSize)
        realloc(size);

    memcpy(m_pBuffer + m_nBufferUseSize, pBuffer, size);
    m_nBufferUseSize += size;
    m_nBufferSpareSize = m_nBufferSize - m_nBufferUseSize;
    m_pBufferUseTail = m_pBuffer + m_nBufferUseSize;
    m_pSeekTag = skp_null;
    m_pSeekLength = skp_null;
    m_pSeekValue = skp_null;
    m_nSeekValueSize = 0;
}

int SkpTLV::appen(int size)
{
    if((uint64)size > m_nBufferSpareSize)
        realloc(size);

    m_nBufferUseSize += size;
    m_nBufferSpareSize = m_nBufferSize - m_nBufferUseSize;
    m_pBufferUseTail = m_pBuffer + m_nBufferUseSize;
    m_pSeekTag = skp_null;
    m_pSeekLength = skp_null;
    m_pSeekValue = m_pBufferUseTail - size;
    m_nSeekValueSize = size;
    return size;
}

int SkpTLV::page(int nSize)
{
    int nPageNumber = nSize / g_nPageSize + 1;
    return nPageNumber * g_nPageSize;
}

char *SkpTLV::seek_value()
{
    return m_pSeekValue;
}

char *SkpTLV::seek_end()
{
    return m_pBufferUseTail;
}

int SkpTLV::tag_encode(TagType *pTagType, uint16 nTag)
{
    uchar nTagSmall = nTag & 0x7f;
    uchar nTagBig = (nTag >> 7) & 0xff;

    pTagType->nHead = nTagBig?SKP_TLV::TAG_TYPE_BIG:SKP_TLV::TAG_TYPE_SMALL;
    pTagType->nSmall = nTagSmall;
    pTagType->nBig = nTagBig;

    return pTagType->nHead + 1;
}

int SkpTLV::tag_decode(TagType *pTagType, uint16 &nTag)
{
    nTag = 0;
    uint16 nData = pTagType->nSmall;
    nTag |= nData;
    if(pTagType->nHead >= SKP_TLV::TAG_TYPE_BIG)
    {
        nData = pTagType->nBig;
        nData <<= 7;
        nTag |= nData;
    }
    return pTagType->nHead + 1;
}

int SkpTLV::length_encode(LengthType *pLengthType, uint nLength)
{
    uchar nOne = nLength & 0x3f;
    uchar nTwo = (nLength >> 6) & 0xff;
    uchar nThree = (nLength >> 14) & 0xff;
    uchar nFour = (nLength >> 22) & 0xff;
    pLengthType->nHead = SKP_TLV::LENGTH_TYPE_ONE;
    if(nTwo)
        pLengthType->nHead = SKP_TLV::LENGTH_TYPE_TWO;
    if(nThree)
        pLengthType->nHead = SKP_TLV::LENGTH_TYPE_THREE;
    if(nFour)
        pLengthType->nHead = SKP_TLV::LENGTH_TYPE_FOUR;

    pLengthType->nOne = nOne;
    pLengthType->nTwo = nTwo;
    pLengthType->nThree = nThree;
    pLengthType->nFour = nFour;


    return pLengthType->nHead + 1;
}

int SkpTLV::length_decode(LengthType *pLengthType, uint &nLength)
{
    nLength = 0;
    uint nData = pLengthType->nOne;
    nLength |= nData;

    if(pLengthType->nHead >= SKP_TLV::LENGTH_TYPE_TWO)
    {
        nData = pLengthType->nTwo;
        nData <<= 6;
        nLength |= nData;
    }
    if(pLengthType->nHead >= SKP_TLV::LENGTH_TYPE_THREE)
    {
        nData = pLengthType->nThree;
        nData <<= 14;
        nLength |= nData;
    }
    if(pLengthType->nHead >= SKP_TLV::LENGTH_TYPE_FOUR)
    {
        nData = pLengthType->nFour;
        nData <<= 22;
        nLength |= nData;
    }

    return pLengthType->nHead + 1;
}

int SkpTLV::add(uint16 nTag, void *pValue, int nLength)
{
    SKP_ASSERT(nLength > 0);
    int nNeedSize = sizeof(TagType) + sizeof(LengthType) + nLength;
    if((uint64)nNeedSize > m_nBufferSpareSize)
    {
        realloc(nNeedSize);
    }
    m_pSeekTag = m_pBufferUseTail;
    TagType *pTagType = (TagType *)m_pSeekTag;
    int nTagSize = tag_encode(pTagType, nTag);
    m_pSeekLength = m_pSeekTag + nTagSize;
    LengthType *pLengthType = (LengthType *)m_pSeekLength;
    int nLengthSize = length_encode(pLengthType, nLength);
    m_pSeekValue = m_pSeekLength + nLengthSize;
    if(pValue)
        memcpy(m_pSeekValue, pValue, nLength);
    m_nSeekValueSize = nLength;
    m_nBufferUseSize += (nTagSize + nLengthSize + nLength);
    m_nBufferSpareSize = m_nBufferSize - m_nBufferUseSize;
    m_pBufferUseTail = m_pBuffer + m_nBufferUseSize;

    return nLength;
}

int SkpTLV::add_null(uint16 nTag,int nLength)
{
    return add(nTag, NULL, nLength);
}

int SkpTLV::seek(uint16 nTag)
{
    uint16 nTLVTag = 0;
    int nTagSize = 0;
    int nLengthSize = 0;
    uint nTLVLength = 0;
    char *pValueStart = m_pBuffer + m_nHeadSize;
    while(pValueStart != m_pBufferUseTail && nTLVTag != nTag)
    {
        m_pSeekTag = pValueStart;
        TagType *pTagType = (TagType *)m_pSeekTag;
        nTLVTag = 0;
        nTagSize = tag_decode(pTagType, nTLVTag);
        m_pSeekLength = m_pSeekTag + nTagSize;
        LengthType *pLengthType = (LengthType *)m_pSeekLength;
        nTLVLength = 0;
        nLengthSize = length_decode(pLengthType, nTLVLength);
        m_pSeekValue = m_pSeekLength + nLengthSize;
        m_nSeekValueSize = nTLVLength;
        pValueStart += (nTagSize + nLengthSize + nTLVLength);
        if(nTag == nTLVTag)
            break;
    }

    return nTLVLength;
}

int SkpTLV::next()
{
    SKP_ASSERT(m_pBuffer);
    SKP_ASSERT(m_pSeekTag);
    SKP_ASSERT(m_pSeekLength);
    SKP_ASSERT(m_pSeekValue);
    SKP_ASSERT(m_nSeekValueSize > 0);
    uint16 nTag = 0;
    TagType *pTagType = (TagType *)m_pSeekTag;
    tag_decode(pTagType, nTag);

    uint16 nTLVTag = 0;
    int nTagSize = 0;
    int nLengthSize = 0;
    uint nTLVLength = 0;
    char *pValueStart = m_pSeekValue + m_nSeekValueSize;
    while(pValueStart != m_pBufferUseTail && nTLVTag != nTag)
    {
        m_pSeekTag = pValueStart;
        TagType *pTagType = (TagType *)m_pSeekTag;
        nTLVTag = 0;
        nTagSize = tag_decode(pTagType, nTLVTag);
        m_pSeekLength = m_pSeekTag + nTagSize;
        LengthType *pLengthType = (LengthType *)m_pSeekLength;
        nTLVLength = 0;
        nLengthSize = length_decode(pLengthType, nTLVLength);
        m_pSeekValue = m_pSeekLength + nLengthSize;
        m_nSeekValueSize = nTLVLength;
        pValueStart += (nTagSize + nLengthSize + nTLVLength);
        if(nTag == nTLVTag)
            break;
    }

    return nTLVLength;
}

int SkpTLV::add_char(uint16 nTag, char nValue)
{
    return add(nTag, (char *)&nValue, sizeof(char));
}

int SkpTLV::add_uchar(uint16 nTag, uchar nValue)
{
    return add(nTag, (char *)&nValue, sizeof(uchar));
}

int SkpTLV::add_short(uint16 nTag, int16 nValue)
{
    return add(nTag, (char *)&nValue, sizeof(int16));
}

int SkpTLV::add_ushort(uint16 nTag, uint16 nValue)
{
    return add(nTag, (char *)&nValue, sizeof(uint16));
}

int SkpTLV::add_int(uint16 nTag, int nValue)
{
    return add(nTag, (char *)&nValue, sizeof(int));
}

int SkpTLV::add_uint(uint16 nTag, uint nValue)
{
    return add(nTag, (char *)&nValue, sizeof(uint));
}

int SkpTLV::add_large_int(uint16 nTag, int64 nValue)
{
    return add(nTag, (char *)&nValue, sizeof(int64));
}

int SkpTLV::add_large_uint(uint16 nTag, uint64 nValue)
{
    return add(nTag, (char *)&nValue, sizeof(uint64));
}

int SkpTLV::add_double(uint16 nTag, double nValue)
{
    return add(nTag, (char *)&nValue, sizeof(double));
}



char SkpTLV::seek_char(uint16 nTag)
{
    char nValue = 0;
    if(seek(nTag))
    {
        char *pSeekValue = seek_value();
        memcpy(&nValue, pSeekValue, sizeof(char));
    }

    return nValue;
}

uchar SkpTLV::seek_uchar(uint16 nTag)
{
    uchar nValue = 0;
    if(seek(nTag))
    {
        char *pSeekValue = seek_value();
        memcpy(&nValue, pSeekValue, sizeof(uchar));
    }

    return nValue;
}

int16 SkpTLV::seek_short(uint16 nTag)
{
    int16 nValue = 0;
    if(seek(nTag))
    {
        char *pSeekValue = seek_value();
        memcpy(&nValue, pSeekValue, sizeof(int16));
    }

    return nValue;
}

uint16 SkpTLV::seek_ushort(uint16 nTag)
{
    uint16 nValue = 0;
    if(seek(nTag))
    {
        char *pSeekValue = seek_value();
        memcpy(&nValue, pSeekValue, sizeof(uint16));
    }

    return nValue;
}

int SkpTLV::seek_int(uint16 nTag)
{
    int nValue = 0;
    if(seek(nTag))
    {
        char *pSeekValue = seek_value();
        memcpy(&nValue, pSeekValue, sizeof(int));
    }

    return nValue;
}

uint SkpTLV::seek_uint(uint16 nTag)
{
    uint nValue = 0;
    if(seek(nTag))
    {
        char *pSeekValue = seek_value();
        memcpy(&nValue, pSeekValue, sizeof(uint));
    }

    return nValue;
}

int64 SkpTLV::seek_large_int(uint16 nTag)
{
    int64 nValue = 0;
    if(seek(nTag))
    {
        char *pSeekValue = seek_value();
        memcpy(&nValue, pSeekValue, sizeof(int64));
    }

    return nValue;
}

uint64 SkpTLV::seek_large_uint(uint16 nTag)
{
    uint64 nValue = 0;
    if(seek(nTag))
    {
        char *pSeekValue = seek_value();
        memcpy(&nValue, pSeekValue, sizeof(uint64));
    }

    return nValue;
}

double SkpTLV::seek_double(uint16 nTag)
{
    double nValue = 0.0;
    if(seek(nTag))
    {
        char *pSeekValue = seek_value();
        memcpy(&nValue, pSeekValue, sizeof(double));
    }

    return nValue;
}
